// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
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

#define DIRECTION (90.f)

// Sets default values
APlayerCharacter::APlayerCharacter()
	: IsControl(true)
	, IsRunning(false)
	, frames(0.f)
	, height(150.f)
	, PlayerState(State_Idle)
	, IsAttacking(false)
	, CanCombo(false)
	, ComboIndex(0)
	, DamageAmount(10.f)
	, HP(100)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// プレイヤーの設定
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// キャラクターコンポーネント取得
	CharacterMovementComp = GetCharacterMovement();
	CharacterMovementComp->MaxWalkSpeed = 500.f;

	// アニメーションセットアップ
	SetupAnimationAsset();

	// 武器のセットアップ
	SetupSword();

	// ボタン設定
	SetupDefoultMappingContext();	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 1);
		}
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
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

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		// ジャンプ
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::JumpStart);

		// 移動
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// ダッシュ
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &APlayerCharacter::Run);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &APlayerCharacter::Run);

		// コンボ1
		EnhancedInputComponent->BindAction(ComboAction, ETriggerEvent::Started, this, &APlayerCharacter::Combo1);

		// コンボ2
		EnhancedInputComponent->BindAction(ComboAction2, ETriggerEvent::Started, this, &APlayerCharacter::Combo2);
	}
}

// ボタンの設定
void APlayerCharacter::SetupDefoultMappingContext()
{
	// パス保存用配列を用意して格納
	TArray<FString> AssetPath;

	AssetPath.Add("/Game/Player/Input/Actions/IA_Move");
	AssetPath.Add("/Game/Player/Input/Actions/IA_Dash");
	AssetPath.Add("/Game/Player/Input/Actions/IA_Jump");
	AssetPath.Add("/Game/Player/Input/Actions/IA_Combo1");
	AssetPath.Add("/Game/Player/Input/Actions/IA_Combo2");

	// ------------------同期ver

	// ボタンのマッピング設定
	DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/0122/Player/Input/IMC_Default"));

	// 各アクションのマッピング
	MoveAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/0122/Player/Input/Actions/IA_Move"));
	RunAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/0122/Player/Input/Actions/IA_Dash"));
	JumpAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/0122/Player/Input/Actions/IA_Jump"));
	ComboAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/0122/Player/Input/Actions/IA_Combo1"));
	ComboAction2 = LoadObject<UInputAction>(nullptr, TEXT("/Game/0122/Player/Input/Actions/IA_Combo2"));

	//// 非同期読み込みリクエストの作成
	//FAsyncLoadCallback AsyncCallback;

	//// 読み込み
	//for (int i = 0; i < AssetPath.Num(); ++i)
	//{

	//	FAsyncLoadCallback AsyncCallback;
	//	AsyncCallback.BindUObject(this, &APlayerCharacter::SetupAnimationAsset);

	//	// アセットの非同期読み込み
	//	StreamableManager.RequestAsyncLoad(AssetPath[i], Delegate);
	//}

}

// アニメーションの設定
void APlayerCharacter::SetupAnimationAsset()
{
	//アニメーションアセットを検索して格納
	ConstructorHelpers::FObjectFinder<UAnimMontage> ComboMontage(TEXT("/Game/0122/Player/Animation/Montage/Combo/SwordCombo"));
	ConstructorHelpers::FObjectFinder<UAnimMontage> ComboMontage2(TEXT("/Game/0122/Player/Animation/Montage/Combo/SwordCombo2"));

	if (ComboMontage.Succeeded())
	{
		ComboAnimMontages.Add(ComboMontage.Object);
	}

	if (ComboMontage2.Succeeded())
	{
		ComboAnimMontages.Add(ComboMontage2.Object);
	}

	// コンボの名前格納
	ComboCntNames = { "First", "Second", "Third"/*,"Fourth"*/};
}

void APlayerCharacter::SetupSword()
{
	// 剣のコンポーネントを作成
	SwordMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

	// 剣のアセット設定
	ConstructorHelpers::FObjectFinder<USkeletalMesh> swordMesh(TEXT("/Game/0122/Player/Weapon/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight"));

	if (swordMesh.Succeeded())
	{
		SwordMesh->SetSkeletalMeshAsset(swordMesh.Object);
	}

	// 体のメッシュに追従
	SwordMesh->SetupAttachment(GetMesh(),"hand_rSocket");

	// 剣の当たり判定作成
	SwordCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollision"));

	// 剣のメッシュに追従
	SwordCollision->SetupAttachment(SwordMesh);
}


void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// コントロール可能か
	if (!IsControl) { return; }

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		// 移動方向に回転
		RotateCharacter(MovementVector.Y);
	}
}

// ダッシュ切り替え
void APlayerCharacter::Run()
{
	// コントロール可能か
	if (!IsControl) { return; }

	if (!IsRunning)
	{
		// ダッシュオン
		IsRunning = true;
		CharacterMovementComp->MaxWalkSpeed = 600.f;
	}
	else
	{
		// ダッシュオン
		IsRunning = false;
		CharacterMovementComp->MaxWalkSpeed = 500.f;
	}

}

void APlayerCharacter::JumpStart()
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

void APlayerCharacter::Jump()
{
	// 現在位置
	FVector NowPos = GetActorLocation();

	// Sinで高さ更新
	float SinValue = height * FMath::Sin(radPerFrame * frames);

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

bool APlayerCharacter::IsPlayerGrounded() const
{
	bool IsGrounded = false;

	// 地面にいるかの確認
	if (CharacterMovementComp)
	{
		IsGrounded = CharacterMovementComp->IsFalling() == false;
	}

	return IsGrounded;
}

// 攻撃
void APlayerCharacter::Attack(int ComboNum /*= 0*/)
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
			if (ComboCntNames[ComboIndex] != ComboCntNames.Last())
			{
				++ComboIndex;
			}
		}
	}

	// 攻撃のアニメーション再生
	PlayAnimation(ComboAnimMontages[ComboNum], ComboCntNames[ComboIndex]);
}

// コンボ1
void APlayerCharacter::Combo1()
{
	// コントロール可能か
	if (!IsControl) { return; }

	// 攻撃
	Attack(0);
}

// コンボ2
void APlayerCharacter::Combo2()
{
	// コントロール可能か
	if (!IsControl) { return; }

	// 攻撃
	Attack(1);
}

void APlayerCharacter::RotateCharacter(float nowInput_Y)
{
	// 入力がない場合は何もしない
	if (nowInput_Y == 0) { return; }

	// 向く方向
	FVector2D Direction;

	// 入力の値に応じて前か後ろを向く
	if (nowInput_Y == 1.f)
	{
		Direction.X = -25.f;
		Direction.Y = DIRECTION;
	}
	else
	{
		Direction.X = 25.f;
		Direction.Y = -DIRECTION;
	}

	// 新しい方向にセット
	const FRotator nowRotate = FRotator(0.f, Direction.Y, Direction.X);

	SetActorRotation(nowRotate);
}


// コンボ継続
void APlayerCharacter::ContinuationCombo()
{
	IsControl = true;
	CanCombo = true;
}


// コンボリセット
void APlayerCharacter::ResetCombo()
{
	// フラグリセット
	IsAttacking = false;
	CanCombo = false;
	IsControl = true;

	// コンボリセット
	ComboIndex = 0;
}

void APlayerCharacter::SetSwordCollision()
{
	// コリジョン判定で無視する項目を指定(今回はこのActor自分自身。thisポインタで指定)
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	// ヒットした(=コリジョン判定を受けた)オブジェクトを格納する変数
	TArray<struct FHitResult> OutHits;


}

void APlayerCharacter::TakedDamage(float _damage)
{
	if (HP)
	{
		// HP計算
		HP -= _damage;

		if (IsAttacking)
		{
			IsAttacking = false;
		}
		
		if (CanCombo)
		{
			CanCombo = false;
		}

		if (!IsControl)
		{
			IsControl = true;
		}

		if (ComboIndex)
		{
			ComboIndex = 0;
		}
	}
	else
	{
		PlayerState = State_Death;
		IsControl = false;
	}
}

void APlayerCharacter::PlayAnimation(UAnimMontage* ToPlayAnimMontage, FName StartSectionName /*= "None"*/)
{
	// コントロール不能へ
	IsControl = false;

	// 再生するアニメーションを格納
	UAnimMontage* toPlayAnimMontage = ToPlayAnimMontage;

	// 何段目か
	FName StartSection = StartSectionName;
	
	// アニメーション再生
	if (toPlayAnimMontage != nullptr)
	{
		PlayAnimMontage(toPlayAnimMontage, 1.f, StartSection);
	}
}