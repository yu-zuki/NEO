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

	// �v���C���[�̐ݒ�
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// �L�����N�^�[�R���|�[�l���g�擾
	CharacterMovementComp = GetCharacterMovement();
	CharacterMovementComp->bOrientRotationToMovement = true;
	CharacterMovementComp->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rat

	// �A�j���[�V�����Z�b�g�A�b�v
	SetupAnimationAsset();

	// �{�^���ݒ�
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

		// �W�����v
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::JumpStart);

		// �ړ�
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// �_�b�V��
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &APlayerCharacter::Run);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &APlayerCharacter::Run);

		// �����؂�ւ�
		EnhancedInputComponent->BindAction(SwitchAction, ETriggerEvent::Started, this, &APlayerCharacter::SwitchAttribute);

		// �R���{1
		EnhancedInputComponent->BindAction(ComboAction, ETriggerEvent::Started, this, &APlayerCharacter::Combo1);

		// �R���{2
		EnhancedInputComponent->BindAction(ComboAction, ETriggerEvent::Started, this, &APlayerCharacter::Combo2);

		// �K�E�Z
		EnhancedInputComponent->BindAction(UltAction, ETriggerEvent::Started, this, &APlayerCharacter::UltimateAttack);
	}
}

// �{�^���̐ݒ�
void APlayerCharacter::SetupDefoultMappingContext()
{
	// �p�X�ۑ��p�z���p�ӂ��Ċi�[
	TArray<FString> AssetPath;

	AssetPath.Add("/Game/Player/Input/Actions/IA_Move");
	AssetPath.Add("/Game/Player/Input/Actions/IA_Dash");
	AssetPath.Add("/Game/Player/Input/Actions/IA_Jump");
	AssetPath.Add("/Game/Player/Input/Actions/IA_SwitchAttribute");
	AssetPath.Add("/Game/Player/Input/Actions/IA_Combo1");
	AssetPath.Add("/Game/Player/Input/Actions/IA_Combo2");
	AssetPath.Add("/Game/Player/Input/Actions/IA_Ult");

	//// �񓯊��ǂݍ��݃��N�G�X�g�̍쐬
	//FAsyncLoadCallback AsyncCallback;

	//// �ǂݍ���
	//for (int i = 0; i < AssetPath.Num(); ++i)
	//{

	//	FAsyncLoadCallback AsyncCallback;
	//	AsyncCallback.BindUObject(this, &APlayerCharacter::SetupAnimationAsset);

	//	// �A�Z�b�g�̔񓯊��ǂݍ���
	//	StreamableManager.RequestAsyncLoad(AssetPath[i], Delegate);
	//}

	// �{�^���̃}�b�s���O�ݒ�
	DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Player/Input/IMC_Default"));
}


// �A�j���[�V�����̐ݒ�
void APlayerCharacter::SetupAnimationAsset()
{
	// �A�j���[�V�����A�Z�b�g���������Ċi�[
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

	// �R���{�̖��O�i�[
	ComboCntNames = { "First", "Second", "Third" };
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// �R���g���[���\��
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

// �_�b�V���؂�ւ�
void APlayerCharacter::Run()
{
	// �R���g���[���\��
	if (!IsControl) { return; }

	IsRunning = (!IsRunning) ? (true) : (false);
}

void APlayerCharacter::JumpStart()
{
	// �R���g���[���\��
	if (!IsControl) { return; }

	// �W�����v�J�n
	if (IsPlayerGrounded())
	{
		// �W�����v����
		frames = 0;

		JumpBeforePos_Z = GetActorLocation().Z;

		PlayerState = State_Jump;
	}
}

void APlayerCharacter::Jump()
{
	// ���݈ʒu
	FVector NowPos = GetActorLocation();

	// Sin�ō����X�V
	float SinValue = height * FMath::Sin(radPerFrame * frames) + JumpBeforePos_Z;


	// �t���[��+1
	frames += 1.f;

	// �ʒu�X�V
	SetActorLocation(FVector(NowPos.X, NowPos.Y, SinValue));
	
	// ���n����
	if (IsPlayerGrounded() && frames >= 20.f)
	{
		PlayerState = State_Idle;
	}
}

bool APlayerCharacter::IsPlayerGrounded() const
{
	bool IsGrounded = false;

	// �n�ʂɂ��邩�̊m�F
	if (CharacterMovementComp)
	{
		IsGrounded = CharacterMovementComp->IsFalling() == false;
	}

	return IsGrounded;
}

// �U��
void APlayerCharacter::Attack()
{
	if (!IsAttacking)
	{
		// �U�����t���O�I��
		IsAttacking = true;

		// �A�j���[�V�����Đ�
		PlayAnimation();
	}
	else
	{
		// �R���{�\�Ȏ�,�p��
		if (CanCombo)
		{
			// �A�j���[�V�����Đ�
			PlayAnimation();
		}
	}
}

// �R���{1
void APlayerCharacter::Combo1()
{
	// �R���g���[���\��
	if (!IsControl) { return; }

	// �X�e�[�g��1�ڂ̃R���{��
	AttackState = State_Combo1;

	// �U��
	Attack();
}

// �R���{2
void APlayerCharacter::Combo2()
{
	// �R���g���[���\��
	if (!IsControl) { return; }

	// �X�e�[�g��2�ڂ̃R���{��
	AttackState = State_Combo2;

	// �U��
	Attack();
}

// �R���{���Z�b�g
void APlayerCharacter::ResetCombo()
{
	// �t���O���Z�b�g
	IsAttacking = false;
	CanCombo = false;

	// �R���{���Z�b�g
	ComboIndex = 0;
}

// �K�E�Z
void APlayerCharacter::UltimateAttack()
{
	// �R���g���[���s�\��
	IsControl = false;

	// �X�e�[�g��K�E�Z��
	AttackState = State_Ult;

	// �K�E�Z�A�j���[�V�����Đ�
	PlayAnimation();
}

void APlayerCharacter::TakedDamage()
{

}

void APlayerCharacter::SwitchAttribute()
{
	// �R���g���[���\��
	if (!IsControl) { return; }

	// �������̎����ɖ߂�
	if (AttrState == State_Wind)
	{
		AttrState = State_Fire;
	}
	else
	{
		// ����ȊO�͏��Ԓʂ�
		AttrState = static_cast<Attribute_State>(AttrState + 1);
	}
}

void APlayerCharacter::PlayAnimation()
{
	// �R���g���[���s�\��
	IsControl = false;

	// �Đ�����A�j���[�V�������i�[
	UAnimMontage* ToPlayAnimMontage = nullptr;

	// �Đ��J�n�ʒu
	FName StartSectionName = "None";

	// �U���̎�ނŃA�j���[�V������؂�ւ�
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
		// �A�j���[�V�����Đ�
		PlayAnimMontage(ToPlayAnimMontage, 1.f, StartSectionName);
	}

	// ���̃R���{��
	++ComboIndex;
}