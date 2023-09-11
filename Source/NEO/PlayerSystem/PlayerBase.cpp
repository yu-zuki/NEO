// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DateTime.h"
#include "Components/InputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NEO/GameSystem/TGS_GameMode.h"
#include "NEO/GameSystem/TGS_GameInstance.h"
#include "PlayerSpline.h"


// Sets default values
APlayerBase::APlayerBase()
	: IsControl(true)
	, IsRunning(false)
	, IsLookRight(true)
	, IsJumping(false)
	, IsCharging(false)
	, IsHoldWeapon(true)
	, IsDeath(false)
	, IsInvincibility(false)
	, frames(0.f)
	, IsAttacking(false)
	, CanCombo(false)
	, ComboIndex(0)
{
	// Tick()をマイフレーム呼ぶかどうかの判定
	PrimaryActorTick.bCanEverTick = true;

	// プレイヤーの設定
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// プレイヤーに回転の制限
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// タグ設定
	Tags.Add("Player");

	// キャラクターコンポーネント取得
	CharacterMovementComp = GetCharacterMovement();
	CharacterMovementComp->MaxWalkSpeed = 500.f;

	// アタックアシストコンポーネント作成
	ActionAssistComp = CreateDefaultSubobject<UActionAssistComponent>(TEXT("AttackAssist"));

	// コリジョンイベントを設定
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerBase::OnOverlap);
}


// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MainActionMapping.DefaultMappingContext, 1);
		}
	}

	// 武器をSpawn
	if (WeaponClass && !Weapon)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, SpawnParams);

		if (Weapon)
		{
			Weapon->AttachToHand(this, "hand_rSocket");
			WeaponType = Weapon->GetWeaponType();
			Weapon->SetOwnerType(EOwnerType::OwnerType_Player);
		}
	}
}


// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ジャンプ中のみ処理
	if (IsJumping)
	{
		Jump();
	}

	// アニメーションに合わせて移動
	if (EnableRootMotion && !ActionAssistComp->WallCheck(10.f))
	{
		RootMotion(AnimationMoveValue);
	}
}


// 入力のバインド
void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 移動
		EnhancedInputComponent->BindAction(MainActionMapping.MoveAction, ETriggerEvent::Triggered, this, &APlayerBase::Move);

		// 走る							
		EnhancedInputComponent->BindAction(MainActionMapping.RunAction, ETriggerEvent::Started, this, &APlayerBase::Run);
		EnhancedInputComponent->BindAction(MainActionMapping.RunAction, ETriggerEvent::Completed, this, &APlayerBase::Run);

		// ジャンプ
		EnhancedInputComponent->BindAction(MainActionMapping.JumpAction, ETriggerEvent::Started, this, &APlayerBase::JumpStart);

		// 攻撃アクション
		EnhancedInputComponent->BindAction(MainActionMapping.ComboAction1, ETriggerEvent::Started, this, &APlayerBase::Attack_Start);
		EnhancedInputComponent->BindAction(MainActionMapping.ComboAction2, ETriggerEvent::Started, this, &APlayerBase::Attack_Start);

		// チャージ終了
		EnhancedInputComponent->BindAction(MainActionMapping.ComboAction1, ETriggerEvent::Completed, this, &APlayerBase::Attack1);
		EnhancedInputComponent->BindAction(MainActionMapping.ComboAction2, ETriggerEvent::Completed, this, &APlayerBase::Attack2);

		
		// チャージ判定用
		//EnhancedInputComponent->BindAction(MainActionMapping.ComboAction1, ETriggerEvent::Completed, this, &APlayerBase::ChargeAttack_End);
		//EnhancedInputComponent->BindAction(MainActionMapping.ComboAction2, ETriggerEvent::Completed, this, &APlayerBase::ChargeAttack_End);
	}
}


/*
 * 関数名　　　　：SetupPlayerData()
 * 処理内容　　　：プレイヤーのデータ初期化
 * 戻り値　　　　：なし
 */
void APlayerBase::SetupPlayerData()
{
	// アクションをマッピング
	SetupMainActionMapping();

	// ステータス設定
	SetupPlayerStatus();

	// コンボの名前格納
	ComboStartSectionNames = { "First", "Second", "Third","Fourth" };

	// ゲームモード取得
	pGameMode = Cast<ATGS_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	// スプラインを検索して格納
	AActor* tempSplineActor = GetSplineActor("PlayerLoad");

	// アクターをスプラインにキャスト
	if(tempSplineActor)
	{
		SplineActor = Cast<APlayerSpline>(tempSplineActor);
	}
}


/*
 * 関数名　　　　：SetupPlayerStatus()
 * 処理内容　　　：プレイヤーのステータス初期化
 * 引数１　　　　：float _hp・・・・・・・・・・・・・・HPの初期値
 * 引数２		 ：int _remainingLife ・・・・・・・・・復活回数
 * 引数３　　　　：float _damageAmount・・・・・・・・・攻撃力の初期値
 * 引数４		 ：float _invincibilityTime_Short ・・・短い無敵時間
 * 引数５        ：float _invincibilityTime_Long  ・・・長い無敵時間
 * 引数６　　　　：float _jumpHeight・・・・・・・・・・ジャンプ力の初期値
 * 引数７　　　　：float _comboDamageFactor ・・・・・・コンボごとのダメージの倍率
 * 引数８　　　　：float _walkSpeed ・・・・・・・・・・歩く速度
 * 引数９　　　　：float _runSpeed  ・・・・・・・・・・走る速度
 * 戻り値　　　　：なし
 */
void APlayerBase::SetupPlayerStatus(float _hp /*= 100.f*/, int _remainingLife /*= 3.f*/, float _damageAmount /*= 10.f*/,
									float _invincibilityTime_Short /*= 0.3f*/, float _invincibilityTime_Long /*= 0.5f*/,
									float _jumpHeight /*= 150.f*/, float _comboDamageFactor /*= 1.f*/, float _walkSpeed /*= 100.f*/, float _runSpeed /*= 300.f*/)
{
	PlayerStatus.HP = _hp;
	PlayerStatus.MaxHP = _hp;
	PlayerStatus.RemainingLife = _remainingLife;
	PlayerStatus.DamageAmount = _damageAmount;
	PlayerStatus.InvincibilityTime_Short = _invincibilityTime_Short;
	PlayerStatus.InvincibilityTime_Long = _invincibilityTime_Long;
	PlayerStatus.JumpHeight = _jumpHeight;
	PlayerStatus.ComboDamageFactor = _comboDamageFactor;
	PlayerStatus.WalkSpeed = _walkSpeed;
	PlayerStatus.RunSpeed = _runSpeed;
	PlayerStatus.NowMoveSpeed = PlayerStatus.WalkSpeed;
}	

/*
 * 関数名　　　　：SetupMainActionMapping()
 * 処理内容　　　：プレイヤーの入力マッピング
 * 戻り値　　　　：なし
 */
void APlayerBase::SetupMainActionMapping()
{
	// ボタン設定
	TCHAR* defaultMappingContext = TEXT("/Game/0122/Player/Input/IMC_MainAction");

	// それぞれのアクションのパス
	TArray<TCHAR*> inputActionPaths;
	inputActionPaths.Add(TEXT("/Game/0122/Player/Input/Actions/IA_Move"));
	inputActionPaths.Add(TEXT("/Game/0122/Player/Input/Actions/IA_Dash"));
	inputActionPaths.Add(TEXT("/Game/0122/Player/Input/Actions/IA_Jump"));
	inputActionPaths.Add(TEXT("/Game/0122/Player/Input/Actions/IA_Combo1"));
	inputActionPaths.Add(TEXT("/Game/0122/Player/Input/Actions/IA_Combo2"));

	// ボタンのマッピング設定
	MainActionMapping.DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, defaultMappingContext);

	// 各アクションのマッピング
	for (int i = 0; i < inputActionPaths.Num(); ++i)
	{
		// 一時保存用
		UInputAction* tempInputAction = LoadObject<UInputAction>(nullptr, inputActionPaths[i]);

		switch (i)
		{
		case 0:
			MainActionMapping.MoveAction = tempInputAction;
			break;
		case 1:
			MainActionMapping.RunAction = tempInputAction;
			break;
		case 2:
			MainActionMapping.JumpAction = tempInputAction;
			break;
		case 3:
			MainActionMapping.ComboAction1 = tempInputAction;
			break;
		case 4:
			MainActionMapping.ComboAction2 = tempInputAction;
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("InputAction Array reference error"));
			break;
		}
	}
}


/*
 * 関数名　　　　：SetupWeaponMesh()
 * 処理内容　　　：プレイヤーのメッシュをセットアップ(引数がStaticMeshの場合)
 * 引数１　　　　：UStaticMeshComponent*& MeshComp・・・メッシュコンポーネント
 * 引数２　　　　：TCHAR* WeaponAssetPath ・・・・・・・武器のアセットのパス
 * 引数３　　　　：FName PublicName ・・・・・・・・・・エディタでの公開名
 * 戻り値　　　　：なし
 */
void APlayerBase::SetupWeaponMesh(UStaticMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName /*= "WeaponMesh"*/)
{
	// 武器のコンポーネントを作成
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(PublicName);

	if (WeaponAssetPath)
	{
		// 武器のアセット設定
		ConstructorHelpers::FObjectFinder< UStaticMesh > weaponMesh(WeaponAssetPath);

		if (weaponMesh.Succeeded())
		{
			MeshComp->SetStaticMesh(weaponMesh.Object);
		}

		// 体のメッシュに追従
		MeshComp->SetupAttachment(GetMesh(), "hand_rSocket");
	}
}


/*
 * 関数名　　　　：SetupWeaponMesh()
 * 処理内容　　　：プレイヤーのメッシュをセットアップ(引数がSkeletalMeshの場合)
 * 引数１　　　　：USkeletalMeshComponent*& MeshComp・・・メッシュコンポーネント
 * 引数２　　　　：TCHAR* WeaponAssetPath ・・・・・・・・武器のアセットのパス
 * 引数３　　　　：FName PublicName ・・・・・・・・・・・エディタでの公開名
 * 戻り値　　　　：なし
 */
void APlayerBase::SetupWeaponMesh(USkeletalMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName /*= "WeaponMesh"*/)
{
	// 武器のコンポーネントを作成
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(PublicName);

	if (WeaponAssetPath)
	{
		// 武器のアセット設定
		ConstructorHelpers::FObjectFinder< USkeletalMesh > weaponMesh(WeaponAssetPath);

		if (weaponMesh.Succeeded())
		{
			MeshComp->SetSkeletalMeshAsset(weaponMesh.Object);
		}

		// 体のメッシュに追従
		MeshComp->SetupAttachment(GetMesh(), "hand_rSocket");
	}
}


/*
 * 関数名　　　　：GetAnimationAsset()
 * 処理内容　　　：アニメーションアセットを返す
 * 引数１　　　　：TCHAR* _animAssetPath ・・・アニメーションアセットのパス
 * 戻り値　　　　：見つかったアニメーション
 */
UAnimMontage* APlayerBase::GetAnimationAsset(TCHAR* _animAssetPath)
{
	// アセットを探してセット
	ConstructorHelpers::FObjectFinder<UAnimMontage> SearchAnimMontage(_animAssetPath);

	// そのアセットが存在したら格納
	if (SearchAnimMontage.Succeeded())
	{
		UAnimMontage* FoundAnimMontage = SearchAnimMontage.Object;

		return FoundAnimMontage;
	}

	return nullptr;
}


/*
 * 関数名　　　　：Move()
 * 処理内容　　　：プレイヤーの入力受付(移動処理)
 * 引数１　　　　：FInputActionValue& Value・・・入力量
 * 戻り値　　　　：なし
 */
void APlayerBase::Move(const FInputActionValue& _value)
{
	// コントロール可能か
	if (!IsControl) { return; }

	// 2軸で入力取得
	FVector2D MovementVector = _value.Get<FVector2D>();

	if (pGameMode)
	{
		// カメラの角度取得
		const FRotator Rotation = pGameMode->GetCameraRotation();

		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		// スプラインに沿った移動方向取得(X,Y)
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 移動
		AddMovementInput(RightDirection, MovementVector.X);
		AddMovementInput(ForwardDirection, MovementVector.Y);

		// 移動方向に回転
		RotateCharacter(MovementVector.Y);
	}
}


/*
 * 関数名　　　　：Run()
 * 処理内容　　　：プレイヤーの入力受付(ダッシュ切り替え)
 * 戻り値　　　　：なし
 */
void APlayerBase::Run()
{
	// コントロール可能か
	if (!IsControl) { return; }

	if (!IsRunning)
	{
		// ダッシュオン
		IsRunning = true;
		PlayerStatus.NowMoveSpeed = PlayerStatus.RunSpeed;
	}
	else
	{
		// ダッシュオン
		IsRunning = false;
		PlayerStatus.NowMoveSpeed = PlayerStatus.WalkSpeed;
	}
}


/*
 * 関数名　　　　：JumpStart()
 * 処理内容　　　：プレイヤーの入力受付(ジャンプ開始)
 * 戻り値　　　　：なし
 */
void APlayerBase::JumpStart()
{
	// コントロール可能か
	if (!IsControl) { return; }

	// 地上にいたらジャンプ開始
	if (!IsJumping)
	{
		// ジャンプ時間
		frames = 0.f;

		JumpBeforePos_Z = GetActorLocation().Z;

		IsJumping = true;
	}
}


/*
 * 関数名　　　　：Jump()
 * 処理内容　　　：プレイヤーのジャンプ中処理
 * 戻り値　　　　：なし
 */
void APlayerBase::Jump()
{
	// 現在位置
	FVector NowPos = GetActorLocation();

	// Sinで高さ更新
	float SinValue = PlayerStatus.JumpHeight * FMath::Sin(radPerFrame * frames);

	// ジャンプ前の高さから位置更新
	const FVector nextPos(FVector(NowPos.X, NowPos.Y, SinValue + JumpBeforePos_Z));

	SetActorLocation(nextPos);

	// 着地処理 下降開始から判定開始
	if (IsPlayerGrounded() && frames >= 20.f)
	{
		IsJumping = false;
	}

	// フレーム+1
	frames += 1.f;
}


/*
 * 関数名　　　　：IsPlayerGrounded()
 * 処理内容　　　：プレイヤーが地面についているか判定
 * 戻り値　　　　：地面についていたらtrue
 */
bool APlayerBase::IsPlayerGrounded() const
{
	bool IsGrounded = false;

	// 地面にいるかの確認
	if (CharacterMovementComp)
	{
		IsGrounded = CharacterMovementComp->IsFalling() == false;
	}

	return IsGrounded;
}


// 接触開始時に行う処理
void APlayerBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// オーバーラップした際に実行したいイベント
	if (OtherActor && (OtherActor != this))
	{
		// 当たったのがプレイヤーの時装備させる
		if (OtherActor->ActorHasTag("Weapon") && !IsHoldWeapon)
		{
			// 新しい武器を作成
			AWeaponBase* NewWeapon = Cast<AWeaponBase>(OtherActor);

			// 武器に所持者がいないかつ空中にいる状態では取れない
			if (!NewWeapon->GetIsHeld() && !NewWeapon->GetIsFalling())
			{
				// 持っている武器を更新
				Weapon = NewWeapon;

				// 武器の種類判別
				WeaponType = Weapon->GetWeaponType();

				// オーナーの種類変更
				Weapon->SetOwnerType(EOwnerType::OwnerType_Player);

				// プレイヤーに装備させる
				Weapon->AttachToHand(this, "hand_rSocket");

				// 武器を落とすまでの回数をリセット
				PlayerStatus.WeaponDropLimit = PlayerStatus.DefaultWeaponDropLimit;

				// 武器を持っている状態に
				IsHoldWeapon = true;
			}
		}
	}
}


/*
 * 関数名　　　　：ComboAttack()
 * 処理内容　　　：プレイヤーの攻撃処理
 * 引数１　　　　：int _attackNum・・・攻撃アニメーションの種類判別用
 * 戻り値　　　　：なし
 */
void APlayerBase::ComboAttack(int _attackNum /*= 0*/)
{
	// コントロール不能へ
	IsControl = false;

	// プレイヤーの角度修正
	ActionAssistComp->CorrectAttackAngle();

	if (!IsJumping)
	{
		if (!IsAttacking)
		{
			// 攻撃中フラグオン
			IsAttacking = true;
		}
		else
		{
			// コンボ可能な時,継続
			if (CanCombo)
			{
				// ラストアタックまでコンボ継続
				if (ComboStartSectionNames[ComboIndex] != ComboStartSectionNames.Last())
				{
					++ComboIndex;
				}
			}
		}

		// 攻撃のアニメーション再生
		PlayAnimation(PlayerAnimation.ComboAttack[_attackNum], ComboStartSectionNames[ComboIndex]);
	}
	else
	{
		// 攻撃のアニメーション再生
		PlayAnimation(PlayerAnimation.AirAttack);
		ComboIndex = 2;
	}
}


/*
 * 関数名　　　　：Attack_Start()
 * 処理内容　　　：プレイヤーの入力受付(溜め攻撃)
 * 戻り値　　　　：なし
 */
void APlayerBase::Attack_Start()
{
	// 攻撃可能か
	if (!IsControl || !IsHoldWeapon) { return; }

	ChargeStartTime = UKismetMathLibrary::Now();

	IsControl = false;
	IsCharging = true;
}

/*
 * 関数名　　　　：Attack1()
 * 処理内容　　　：プレイヤーの入力受付(攻撃１つ目)
 * 戻り値　　　　：なし
 */
void APlayerBase::Attack1()
{
	if (!IsCharging) { return; }

	IsCharging = false;

	FDateTime ChargeEndTime = UKismetMathLibrary::Now();

	// 差分を計算          
	FTimespan Span = ChargeEndTime - ChargeStartTime;

	// ボタンを押している時間で攻撃変更
	if (Span.GetSeconds() <= ChargeTime)
	{
		// コンボ攻撃
		ComboAttack(0);
	}
	else
	{
		// 溜め攻撃
		ChargeAttack();
	}
}

/*
 * 関数名　　　　：Attack2()
 * 処理内容　　　：プレイヤーの入力受付(攻撃２つ目)
 * 戻り値　　　　：なし
 */
void APlayerBase::Attack2()
{
	if (!IsCharging) { return; }

	IsCharging = false;

	FDateTime ChargeEndTime = UKismetMathLibrary::Now();

	// 差分を計算          
	FTimespan Span = ChargeEndTime - ChargeStartTime;

	// ボタンを押している時間で攻撃変更
	if (Span.GetSeconds() <= ChargeTime)
	{
		// コンボ攻撃
		ComboAttack(1);
	}
	else
	{
		// 溜め攻撃
		ChargeAttack();
	}
}


/*
 * 関数名　　　　：ChargeAttack()
 * 処理内容　　　：溜め攻撃
 * 戻り値　　　　：なし
 */
void APlayerBase::ChargeAttack()
{
	PlayAnimation(PlayerAnimation.ChargeAttack);
}

/*
 * 関数名　　　　：RotateCharacter()
 * 処理内容　　　：プレイヤーのステータス初期化
 * 引数１　　　　：float _nowInput_X・・・現在の移動入力値
 * 戻り値　　　　：なし
 */
void APlayerBase::RotateCharacter(float _nowInput_X)
{
	// 入力がない場合は何もしない
	if (_nowInput_X == 0) { return; }

	// 右を向いているか確認
	IsLookRight = (_nowInput_X != 1.f) ? (true) : (false);

	// 回転
	ActionAssistComp->OwnerParallelToCamera(IsLookRight);
}


/*
 * 関数名　　　　：RotateCharacter()
 * 処理内容　　　：キャラクターの移動量取得
 * 引数１　　　　：FVector _nowPos・・・現在位置
 * 戻り値　　　　：なし
 */
void APlayerBase::AmountOfMovement(FVector _nowPos)
{
	// 同じ位置にいたらスキップ
	if (BeforePos == _nowPos) { return; }

	// 距離計算
	const float AmountOfMovement = FVector::Dist(_nowPos, BeforePos);

	// 移動量保存
	DistanceAdvanced += AmountOfMovement;

	// 現在の位置を保存
	BeforePos = _nowPos;
}


/*
 * 関数名　　　　：SlowDownDeathAnimationRate()
 * 処理内容　　　：死亡時アニメーション引き伸ばし
 * 戻り値　　　　：なし
 */
void APlayerBase::SlowDownDeathAnimationRate()
{
	// 遅くする
	GetMesh()->GlobalAnimRateScale = DeadAnimRate;


	// プレイヤーを削除
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(TimerHandle, this, &APlayerBase::CallGameModeFunc_DestroyPlayer, DeadToGameOverTime, false);
}



/*
 * 関数名　　　　：SetEnableRootMotion()
 * 処理内容　　　：RootMotionの開始と移動距離
 * 引数１　　　　：bool _enableRootMotion・・・ルートモーションのオンオフ
 * 引数２　　　　：float _distance・・・・・・・アニメーションで移動した距離
 * 戻り値　　　　：なし
 */
void APlayerBase::SetEnableRootMotion(bool _enableRootMotion, float _distance /*= 0*/)
{
	// アニメーション移動開始
	EnableRootMotion = _enableRootMotion; 

	// アニメーションでの移動値
	AnimationMoveValue = _distance;
}


/*
 * 関数名　　　　：RootMotion()
 * 処理内容　　　：RootMotionの疑似的な実装
 * 引数１　　　　：float _distance・・・アニメーションで移動した距離
 * 戻り値　　　　：なし
 */
void APlayerBase::RootMotion(float _distance)
{
	// 向いている方向によって値を変換
	const float Distance = (IsLookRight) ? (-_distance) : (_distance);

	// 前方ベクトル取得
	FVector ForwardVector = GetActorForwardVector();

	// 現在向いている方向に移動する
	FVector newVector = ForwardVector * _distance;

	// 加算
	AddActorWorldOffset(newVector,false);
}


/*
 * 関数名　　　　：CallGameModeFunc_DestroyPlayer()
 * 処理内容　　　：死亡時のゲームモード内の関数呼び出し
 * 　　　　　　　　残機が残ってたらリスポーン,なくなったらプレイヤー削除
 * 戻り値　　　　：なし
 */
void APlayerBase::CallGameModeFunc_DestroyPlayer()
{

	// ゲームモード作成
	ATGS_GameMode* gameMode = Cast<ATGS_GameMode>(GetWorld()->GetAuthGameMode());
	UTGS_GameInstance* GameInstance = GetGameInstance();

	if (gameMode)
	{
		gameMode->DestroyPlayer(this);

		PlayerStatus.RemainingLife = GameInstance->LoadRemainingLife();

		// 残機があるうちはリスポーン
		if (PlayerStatus.RemainingLife > 0)
		{
			// プレイヤーリスポーン
			gameMode->RespawnPlayer();

			// 残機-１
			GameInstance->SaveRemainingLife( --PlayerStatus.RemainingLife );
		}
		// それ以外はゲームオーバー
		else
		{
			// ゲームオーバーへ
			gameMode->SetState_GameOver();

			// プレイヤー削除
			gameMode->DestroyPlayer(this);
		}
	}
}


/*
 * 関数名　　　　：ResetAllAttackFlags()
 * 処理内容　　　：攻撃に関するフラグをすべてリセット
 * 戻り値　　　　：なし
 */void APlayerBase::ResetAllAttackFlags()
{
	// 攻撃中のフラグリセット
	IsAttacking = false;
	IsCharging = false;
	CanCombo = false;
	IsControl = false;
	ComboIndex = 0;
}


/*
 * 関数名　　　　：GetSplineActor()
 * 処理内容　　　：SplineActor検索
 * 引数１　　　　：FName _tag・・・このタグを持ったActorを検索
 * 戻り値　　　　：なし
 */
AActor* APlayerBase::GetSplineActor(const FName _tag)
{
	//ゲーム全体に対するActorの検索コストが高いため、一回保存しておくだけにする
	//検索対象は全てのActor
	TSubclassOf<AActor> findClass;
	findClass = AActor::StaticClass();
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), findClass, actors);

	//検索結果、Actorがあれば
	if (actors.Num() > 0)
	{
		// そのActorの中を順番に検索
		for (int idx = 0; idx < actors.Num(); idx++)
		{
			AActor* pActor = Cast<AActor>(actors[idx]);

			// タグ名で判断
			if (pActor->ActorHasTag(_tag))
			{
				//デバッグ確認
				FString message = FString("Founded Actor	:") + pActor->GetName();
				UE_LOG(LogTemp, Warning, TEXT("%s"), *message);


				return pActor;
			}
		}
	}


	return NULL;
}


/*
 * 関数名　　　　：ContinuationCombo()
 * 処理内容　　　：コンボの継続
 * 戻り値　　　　：なし
 */
void APlayerBase::ContinuationCombo()
{
	IsControl = true;
	CanCombo = true;
}


/*
 * 関数名　　　　：ResetCombo()
 * 処理内容　　　：コンボリセット
 * 戻り値　　　　：なし
 */
void APlayerBase::ResetCombo()
{
	// 攻撃中のフラグをすべてリセット
	ResetAllAttackFlags();

	// コントロール可能状態へ
	IsControl = true;
}


/*
 * 関数名　　　　：TakedDamage()
 * 処理内容　　　：プレイヤーの被ダメージ処理
 * 引数１　　　　：float _damage・・・被ダメージ量
 * 引数２　　　　：bool _isLastAttack・・・コンボの最終かどうか
 * 戻り値　　　　：なし
 */
void APlayerBase::TakedDamage(float _damage, bool _isLastAttack /*= false*/)
{
	if (IsDeath || IsInvincibility) { return; }

	// 武器を持っていないときに攻撃を受けたら死亡
	if (!IsHoldWeapon && !IsDeath)
	{
		// コントロール不能へ
		IsControl = false;

		// 死亡状態へ
		IsDeath = true;

		// コリジョンをオフに
		SetActorEnableCollision(true);

		// ヒットエフェクト発生
		ActionAssistComp->SpawnEffect(HitEffect, GetActorLocation());

		// 死亡アニメーション再生
		PlayAnimation(PlayerAnimation.Death);
	}
	// 武器を持っているとき
	else
	{
		// 攻撃中のフラグをすべてリセット
		ResetAllAttackFlags();

		// 敵のコンボが最終段だった時必ず武器を落とす
		if (_isLastAttack)
		{
			PlayerStatus.WeaponDropLimit = 0;
		}

		// 無敵解除時間格納用
		float InvincibilityReleaseTime;

		// 被ダメージアニメーション
		if (PlayerStatus.WeaponDropLimit <= 0)
		{
			// 長い無敵時間を適用
			InvincibilityReleaseTime = PlayerStatus.InvincibilityTime_Long;

			// 武器を落とす
			if (Weapon && IsHoldWeapon)
			{
				Weapon->DetachToHand();
				Weapon = nullptr;
				IsHoldWeapon = false;
			}

			// ノックバックアニメーション再生
			PlayAnimation(PlayerAnimation.KnockBack);
		}
		else
		{
			// 短い無敵時間を適用
			InvincibilityReleaseTime = PlayerStatus.InvincibilityTime_Long;

			// 攻撃を受ける
			--PlayerStatus.WeaponDropLimit;

			// のけぞりアニメーション再生
			PlayAnimation(PlayerAnimation.TakeDamage);			
		}

		// ヒットエフェクト発生
		ActionAssistComp->SpawnEffect(HitEffect, GetActorLocation());

		// 無敵開始
		IsInvincibility = true;

		// 任意の時間後無敵解除
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		TimerManager.SetTimer(TimerHandle, this, &APlayerBase::InvincibilityRelease, InvincibilityReleaseTime, false);
	}
}


/*
 * 関数名　　　　：PlayAnimation()
 * 処理内容　　　：プレイヤーのアニメーション再生(再生中は操作不可)
 * 戻り値　　　　：なし
 */
void APlayerBase::PlayAnimation(UAnimMontage* _toPlayAnimMontage, FName _startSectionName /*= "None"*/, float _playRate /*= 1.f*/)
{
	// コントロール不能へ
	IsControl = false;

	// 再生するアニメーションを格納
	UAnimMontage* toPlayAnimMontage = _toPlayAnimMontage;

	// アニメーション再生
	if (toPlayAnimMontage != nullptr)
	{
		ActionAssistComp->PlayAnimation(_toPlayAnimMontage, _startSectionName, _playRate);
	}
}

UTGS_GameInstance* APlayerBase::GetGameInstance()
{
	UWorld* World = GetWorld();
	if (!World) return nullptr;

	UTGS_GameInstance* GameInstance = Cast<UTGS_GameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance) {
		return GameInstance;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("GameInstance is not Found"));
	}
	return nullptr;
}


