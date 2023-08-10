// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/StreamableManager.h"
#include "Engine/World.h"
#include "Engine/AssetManager.h"
#include "Async/Async.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NEO/GameSystem/TGS_GameMode.h"
#include "Components/WidgetComponent.h"
#include "NEO/GameSystem/TGS_GameInstance.h"
#include "PlayerSpline.h"
#include "NiagaraComponent.h"
#include "ActionAssistComponent.h"


// Sets default values
APlayerBase::APlayerBase()
	: IsControl(true)
	, IsRunning(false)
	, frames(0.f)
	, PlayerState(State_Idle)
	, IsAttacking(false)
	, CanCombo(false)
	, ComboIndex(0)
	, DeadAnimRate(0.01f)
	, DeadToGameOverTime(3.f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

}


// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (PlayerState)
	{
	case State_Idle:
		break;
	case State_Jump:
		Jump();
		break;
	case State_Death:
		break;
	}

	deltaTime = DeltaTime;
}


// Called to bind functionality to input
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

		// コンボアクション
		EnhancedInputComponent->BindAction(MainActionMapping.ComboAction1, ETriggerEvent::Started, this, &APlayerBase::Combo1);
		EnhancedInputComponent->BindAction(MainActionMapping.ComboAction2, ETriggerEvent::Started, this, &APlayerBase::Combo2);
	}
}


/*
 * 関数名　　　　：SetupPlayerData()
 * 処理内容　　　：プレイヤーのデータ初期化
 * 戻り値　　　　：なし
 */
void APlayerBase::SetupPlayerData()
{
	// メインアクションのボタンをマッピング
	SetupMainActionMapping();

	// コンボの名前格納
	ComboStartSectionNames = { "First", "Second", "Third"/*,"Fourth"*/ };

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
 * 引数１　　　　：float _hp・・・・・・・・・HPの初期値
 * 引数２		 ：int _remainingLife ・・・・復活回数
 * 引数３　　　　：float _damageAmount・・・・攻撃力の初期値
 * 引数４　　　　：float _jumpHeight・・・・・ジャンプ力の初期値
 * 引数５　　　　：float _comboDamageFactor・コンボごとのダメージの倍率
 * 引数６　　　　：float _walkSpeed・・・・・歩く速度
 * 引数７　　　　：float _runSpeed ・・・・・走る速度
 * 戻り値　　　　：なし
 */
void APlayerBase::SetupPlayerStatus(float _hp /*= 100.f*/, int _remainingLife /*= 3.f*/, float _damageAmount /*= 10.f*/,
									float _jumpHeight /*= 150.f*/, float _comboDamageFactor /*= 1.f*/, float _walkSpeed /*= 100.f*/, float _runSpeed /*= 300.f*/)
{
	PlayerStatus.HP = _hp;
	PlayerStatus.MaxHP = _hp;
	PlayerStatus.RemainingLife = _remainingLife;
	PlayerStatus.DamageAmount = _damageAmount;
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

	// input is a Vector2D
	FVector2D MovementVector = _value.Get<FVector2D>();
	
	if (SplineActor)
	{
		// スプラインの角度取得
		const FRotator Rotation = SplineActor->GetSplineAngle(DistanceAdvanced * CharacterMovementComp->MaxWalkSpeed * deltaTime);

		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		// 移動方向取得(X,Y)
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		

		// 移動
		AddMovementInput(RightDirection, MovementVector.X);
		AddMovementInput(ForwardDirection, MovementVector.Y);

		// 移動方向に回転
		RotateCharacter(MovementVector.X);

		// 移動量保存
		if (!ActionAssistComp->WallCheck())
		{
			DistanceAdvanced += MovementVector.X;

		}

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
	if (IsPlayerGrounded())
	{
		// ジャンプ時間
		frames = 0.f;

		JumpBeforePos_Z = GetActorLocation().Z;

		PlayerState = State_Jump;
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
		PlayerState = State_Idle;
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


/*
 * 関数名　　　　：Attack()
 * 処理内容　　　：プレイヤーの攻撃処理
 * 引数１　　　　：int _attackNum・・・攻撃アニメーションの種類判別用
 * 戻り値　　　　：なし
 */
void APlayerBase::Attack(int _attackNum /*= 0*/)
{
	// プレイヤーの角度修正
	ActionAssistComp->CorrectAttackAngle();

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
	PlayAnimation(PlayerAnimation.Combo[_attackNum],ComboStartSectionNames[ComboIndex]);
}


/*
 * 関数名　　　　：Combo1()
 * 処理内容　　　：プレイヤーの入力受付(攻撃１つ目)
 * 戻り値　　　　：なし
 */
void APlayerBase::Combo1()
{
	// コントロール可能か
	if (!IsControl) { return; }

	// 攻撃
	Attack(0);
}

/*
 * 関数名　　　　：Combo2()
 * 処理内容　　　：プレイヤーの入力受付(攻撃２つ目)
 * 戻り値　　　　：なし
 */
void APlayerBase::Combo2()
{
	// コントロール可能か
	if (!IsControl) { return; }

	// 攻撃
	Attack(1);
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
	bool LookRight = (_nowInput_X != 1.f) ? (true) : (false);

	// 回転
	ActionAssistComp->OwnerParallelToCamera(LookRight);

}


/*
 * 関数名　　　　：SlowDawnDeathAnimationRate()
 * 処理内容　　　：死亡時アニメーション引き伸ばし
 * 戻り値　　　　：なし
 */
void APlayerBase::SlowDownDeathAnimationRate()
{
	// 遅くする
	GetMesh()->GlobalAnimRateScale = DeadAnimRate;


	// プレイヤーを削除
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(TimerHandle_DeathToGameOver, this, &APlayerBase::CallGameModeFunc_DestroyPlayer, DeadToGameOverTime, false);
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

//タグからActorを取得
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
	// フラグリセット
	IsAttacking = false;
	CanCombo = false;
	IsControl = true;

	// コンボリセット
	ComboIndex = 0;
}

/*
 * 関数名　　　　：TakedDamage()
 * 処理内容　　　：プレイヤーの被ダメージ処理
 * 引数１　　　　：float _damage・・・被ダメージ量
 * 戻り値　　　　：なし
 */
void APlayerBase::TakedDamage(float _damage)
{
	if (PlayerStatus.HP >= 0.f)
	{
		// HP計算
		PlayerStatus.HP -= _damage;

		// HPが0以下になったら
		if (PlayerStatus.HP <= 0.f)
		{
			// HPが0以下なら死
			PlayerState = State_Death;

			IsControl = false;

			// 死亡アニメーション再生
			SetActorEnableCollision(true);

			// ヒットエフェクト発生
			ActionAssistComp->SpawnHitEffect(HitEffect, GetActorLocation());

			PlayAnimation(PlayerAnimation.Death);
		}
		else
		{
			// 攻撃中のフラグリセット
			if (IsAttacking)
			{
				IsAttacking = false;
				CanCombo = false;
				IsControl = true;
				ComboIndex = 0;
			}

			// ヒットエフェクト発生
			ActionAssistComp->SpawnHitEffect(HitEffect,GetActorLocation());

			// 被ダメージアニメーション再生
			PlayAnimation(PlayerAnimation.TakeDamage);
		}
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


