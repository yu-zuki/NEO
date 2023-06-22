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

// Sets default values
APlayerCharacter::APlayerCharacter()
	: IsControl(true)
	, IsRunning(false)
	, frames(0.f)
	, PlayerState(State_Idle)
	, AttrState(State_Fire)
	, IsAttacking(false)
	, AttackState(State_Combo1)
	, CanCombo(false)
	, ComboIndex(0)
	, DamageAmount(2.f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// プレイヤーの設定
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// キャラクターコンポーネント取得
	CharacterMovementComp = GetCharacterMovement();
	CharacterMovementComp->bOrientRotationToMovement = true;
	CharacterMovementComp->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rat

	// アニメーションセットアップ
	SetupAnimationAsset();

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
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
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
	}

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		// ジャンプ
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::JumpStart);

		// 移動
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// ダッシュ
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &APlayerCharacter::Run);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &APlayerCharacter::Run);

		// 属性切り替え
		EnhancedInputComponent->BindAction(SwitchAction, ETriggerEvent::Started, this, &APlayerCharacter::SwitchAttribute);

		// コンボ1
		EnhancedInputComponent->BindAction(ComboAction, ETriggerEvent::Started, this, &APlayerCharacter::Combo1);

		// コンボ2
		EnhancedInputComponent->BindAction(ComboAction, ETriggerEvent::Started, this, &APlayerCharacter::Combo2);

		// 必殺技
		EnhancedInputComponent->BindAction(UltAction, ETriggerEvent::Started, this, &APlayerCharacter::UltimateAttack);
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
	AssetPath.Add("/Game/Player/Input/Actions/IA_SwitchAttribute");
	AssetPath.Add("/Game/Player/Input/Actions/IA_Combo1");
	AssetPath.Add("/Game/Player/Input/Actions/IA_Combo2");
	AssetPath.Add("/Game/Player/Input/Actions/IA_Ult");

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

	// ボタンのマッピング設定
	DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Player/Input/IMC_Default"));
}


// アニメーションの設定
void APlayerCharacter::SetupAnimationAsset()
{
	// アニメーションアセットを検索して格納
	//ConstructorHelpers::FObjectFinder<UAnimMontage> ComboMontage(TEXT("/Game/Player/Animation/Montage/Combo/sword/SwordCombo"));
	//ConstructorHelpers::FObjectFinder<UAnimMontage> ComboMontage2(TEXT("/Game/Player/Animation/Montage/Combo/sword/SwordCombo2"));
	//ConstructorHelpers::FObjectFinder<UAnimMontage> UltMontage(TEXT("/Game/Player/Animation/Montage/hissatu"));

	//if (ComboMontage.Succeeded())
	//{
	//	ComboAnimMontages.Add(ComboMontage.Object);
	//}

	//if (ComboMontage2.Succeeded())
	//{
	//	ComboAnimMontages.Add(ComboMontage2.Object);
	//}

	//if (UltMontage.Succeeded())
	//{
	//	UltAnimMontage = UltMontage.Object;
	//}

	// コンボの名前格納
	ComboCntNames = { "First", "Second", "Third" };
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
	}
}

// ダッシュ切り替え
void APlayerCharacter::Run()
{
	// コントロール可能か
	if (!IsControl) { return; }

	IsRunning = (!IsRunning) ? (true) : (false);
}

void APlayerCharacter::JumpStart()
{
	// コントロール可能か
	if (!IsControl) { return; }

	// ジャンプ開始
	if (IsPlayerGrounded())
	{
		// ジャンプ時間
		frames = 0;

		JumpBeforePos_Z = GetActorLocation().Z;

		PlayerState = State_Jump;
	}
}

void APlayerCharacter::Jump()
{
	// 現在位置
	FVector NowPos = GetActorLocation();

	// Sinで高さ更新
	float SinValue = height * FMath::Sin(radPerFrame * frames) + JumpBeforePos_Z;


	// フレーム+1
	frames += 1.f;

	// 位置更新
	SetActorLocation(FVector(NowPos.X, NowPos.Y, SinValue));
	
	// 着地処理
	if (IsPlayerGrounded() && frames >= 20.f)
	{
		PlayerState = State_Idle;
	}
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
void APlayerCharacter::Attack()
{
	if (!IsAttacking)
	{
		// 攻撃中フラグオン
		IsAttacking = true;

		// アニメーション再生
		PlayAnimation();
	}
	else
	{
		// コンボ可能な時,継続
		if (CanCombo)
		{
			// アニメーション再生
			PlayAnimation();
		}
	}
}

// コンボ1
void APlayerCharacter::Combo1()
{
	// コントロール可能か
	if (!IsControl) { return; }

	// ステートを1つ目のコンボへ
	AttackState = State_Combo1;

	// 攻撃
	Attack();
}

// コンボ2
void APlayerCharacter::Combo2()
{
	// コントロール可能か
	if (!IsControl) { return; }

	// ステートを2つ目のコンボへ
	AttackState = State_Combo2;

	// 攻撃
	Attack();
}

// コンボリセット
void APlayerCharacter::ResetCombo()
{
	// フラグリセット
	IsAttacking = false;
	CanCombo = false;

	// コンボリセット
	ComboIndex = 0;
}

// 必殺技
void APlayerCharacter::UltimateAttack()
{
	// コントロール不能へ
	IsControl = false;

	// ステートを必殺技へ
	AttackState = State_Ult;

	// 必殺技アニメーション再生
	PlayAnimation();
}

void APlayerCharacter::TakedDamage()
{

}

void APlayerCharacter::SwitchAttribute()
{
	// コントロール可能か
	if (!IsControl) { return; }

	// 風属性の時炎に戻る
	if (AttrState == State_Wind)
	{
		AttrState = State_Fire;
	}
	else
	{
		// それ以外は順番通り
		AttrState = static_cast<Attribute_State>(AttrState + 1);
	}
}

void APlayerCharacter::PlayAnimation()
{
	// コントロール不能へ
	IsControl = false;

	// 再生するアニメーションを格納
	UAnimMontage* ToPlayAnimMontage = nullptr;

	// 再生開始位置
	FName StartSectionName = "None";

	// 攻撃の種類でアニメーションを切り替え
	switch (AttackState)
	{
	case State_Combo1:
		ToPlayAnimMontage = ComboAnimMontages[State_Combo1];
		StartSectionName = ComboCntNames[ComboIndex];
		break;
	case State_Combo2:
		ToPlayAnimMontage = ComboAnimMontages[State_Combo2];
		StartSectionName = ComboCntNames[ComboIndex];
		break;
	case State_Ult:
		ToPlayAnimMontage = UltAnimMontage;
		break;
	}

	if (ToPlayAnimMontage != nullptr)
	{
		// アニメーション再生
		PlayAnimMontage(ToPlayAnimMontage, 1.f, StartSectionName);
	}

	// 次のコンボへ
	++ComboIndex;
}