// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
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
APlayerBase::APlayerBase()
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
			Subsystem->AddMappingContext(InputMapping.DefaultMappingContext, 1);
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

}

// Called to bind functionality to input
void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 移動
		EnhancedInputComponent->BindAction(InputMapping.MoveAction, ETriggerEvent::Triggered, this, &APlayerBase::Move);

		// 走る
		EnhancedInputComponent->BindAction(InputMapping.RunAction, ETriggerEvent::Started, this, &APlayerBase::Run);
		EnhancedInputComponent->BindAction(InputMapping.RunAction, ETriggerEvent::Completed, this, &APlayerBase::Run);

		// ジャンプ
		EnhancedInputComponent->BindAction(InputMapping.JumpAction, ETriggerEvent::Started, this, &APlayerBase::JumpStart);

		// コンボアクション
		EnhancedInputComponent->BindAction(InputMapping.ComboAction1, ETriggerEvent::Started, this, &APlayerBase::Combo1);
		EnhancedInputComponent->BindAction(InputMapping.ComboAction2, ETriggerEvent::Started, this, &APlayerBase::Combo2);
	}
}

// ボタンの設定
void APlayerBase::SetupDefoultMappingContext(TCHAR* MappingAssetPath, TArray<TCHAR*> InputActionAssetPaths)
{
	// ボタンのマッピング設定
	InputMapping.DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, MappingAssetPath);

	// 各アクションのマッピング
	for (int i = 0; i < InputActionAssetPaths.Num(); ++i)
	{
		// 一時保存用
		UInputAction* tempInputAction = LoadObject<UInputAction>(nullptr, InputActionAssetPaths[i]);

		switch (i)
		{
		case 1:
			InputMapping.MoveAction = tempInputAction;
			break;
		case 2:
			InputMapping.RunAction = tempInputAction;
			break;
		case 3:
			InputMapping.JumpAction = tempInputAction;
			break;
		case 4:
			InputMapping.ComboAction1 = tempInputAction;
			break;
		case 5:
			InputMapping.ComboAction2 = tempInputAction;
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("InputAction Array reference error"));
			break;

		}
	}
}

// アニメーションの設定
void APlayerBase::SetupAnimationAsset(TCHAR* AnimAssetPath[2])
{
	// アセットを探してセット
	for (int i = 0; i < 2; ++i)
	{
		ConstructorHelpers::FObjectFinder<UAnimMontage> ComboMontage(AnimAssetPath[i]);

		if (ComboMontage.Succeeded())
		{
			ComboAnimMontages.Add(ComboMontage.Object);
		}
	}

	// コンボの名前格納
	ComboCntNames = { "First", "Second", "Third"/*,"Fourth"*/ };
}

void APlayerBase::SetupWeapon(TCHAR* WeaponAssetPath, FName PublicName/* = "Weapon"*/)
{
	// 武器のコンポーネントを作成
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(PublicName);

	if (WeaponAssetPath)
	{
		// 武器のアセット設定
		ConstructorHelpers::FObjectFinder<USkeletalMesh> weaponMesh(WeaponAssetPath);

		if (weaponMesh.Succeeded())
		{
			WeaponMesh->SetSkeletalMeshAsset(weaponMesh.Object);
		}

		// 体のメッシュに追従
		WeaponMesh->SetupAttachment(GetMesh(), "hand_rSocket");
	}
}

void APlayerBase::SetupCollision()
{
	// 剣の当たり判定作成
	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollision"));

	// 剣のメッシュに追従
	WeaponCollision->SetupAttachment(WeaponMesh);
}



void APlayerBase::Move(const FInputActionValue& Value)
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
void APlayerBase::Run()
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

void APlayerBase::Jump()
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

// 攻撃
void APlayerBase::Attack(int AttackNum /*= 0*/)
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
	PlayAnimation(ComboAnimMontages[AttackNum], ComboCntNames[ComboIndex]);
}

// コンボ1
void APlayerBase::Combo1()
{
	// コントロール可能か
	if (!IsControl) { return; }

	// 攻撃
	Attack(0);
}

// コンボ2
void APlayerBase::Combo2()
{
	// コントロール可能か
	if (!IsControl) { return; }

	// 攻撃
	Attack(1);
}

void APlayerBase::RotateCharacter(float nowInput_Y)
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
void APlayerBase::ContinuationCombo()
{
	IsControl = true;
	CanCombo = true;
}


// コンボリセット
void APlayerBase::ResetCombo()
{
	// フラグリセット
	IsAttacking = false;
	CanCombo = false;
	IsControl = true;

	// コンボリセット
	ComboIndex = 0;
}

void APlayerBase::SetCollision()
{
	// コリジョン判定で無視する項目を指定(今回はこのActor自分自身。thisポインタで指定)
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	// ヒットした(=コリジョン判定を受けた)オブジェクトを格納する変数
	TArray<struct FHitResult> OutHits;


}

void APlayerBase::TakedDamage(float _damage)
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

void APlayerBase::PlayAnimation(UAnimMontage* ToPlayAnimMontage, FName StartSectionName /*= "None"*/)
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