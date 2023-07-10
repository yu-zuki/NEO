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

	// �v���C���[�̐ݒ�
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// �L�����N�^�[�R���|�[�l���g�擾
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
		// �ړ�
		EnhancedInputComponent->BindAction(InputMapping.MoveAction, ETriggerEvent::Triggered, this, &APlayerBase::Move);

		// ����
		EnhancedInputComponent->BindAction(InputMapping.RunAction, ETriggerEvent::Started, this, &APlayerBase::Run);
		EnhancedInputComponent->BindAction(InputMapping.RunAction, ETriggerEvent::Completed, this, &APlayerBase::Run);

		// �W�����v
		EnhancedInputComponent->BindAction(InputMapping.JumpAction, ETriggerEvent::Started, this, &APlayerBase::JumpStart);

		// �R���{�A�N�V����
		EnhancedInputComponent->BindAction(InputMapping.ComboAction1, ETriggerEvent::Started, this, &APlayerBase::Combo1);
		EnhancedInputComponent->BindAction(InputMapping.ComboAction2, ETriggerEvent::Started, this, &APlayerBase::Combo2);
	}
}

// �{�^���̐ݒ�
void APlayerBase::SetupDefoultMappingContext(TCHAR* MappingAssetPath, TArray<TCHAR*> InputActionAssetPaths)
{
	// �{�^���̃}�b�s���O�ݒ�
	InputMapping.DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, MappingAssetPath);

	// �e�A�N�V�����̃}�b�s���O
	for (int i = 0; i < InputActionAssetPaths.Num(); ++i)
	{
		// �ꎞ�ۑ��p
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

// �A�j���[�V�����̐ݒ�
void APlayerBase::SetupAnimationAsset(TCHAR* AnimAssetPath[2])
{
	// �A�Z�b�g��T���ăZ�b�g
	for (int i = 0; i < 2; ++i)
	{
		ConstructorHelpers::FObjectFinder<UAnimMontage> ComboMontage(AnimAssetPath[i]);

		if (ComboMontage.Succeeded())
		{
			ComboAnimMontages.Add(ComboMontage.Object);
		}
	}

	// �R���{�̖��O�i�[
	ComboCntNames = { "First", "Second", "Third"/*,"Fourth"*/ };
}

void APlayerBase::SetupWeapon(TCHAR* WeaponAssetPath, FName PublicName/* = "Weapon"*/)
{
	// ����̃R���|�[�l���g���쐬
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(PublicName);

	if (WeaponAssetPath)
	{
		// ����̃A�Z�b�g�ݒ�
		ConstructorHelpers::FObjectFinder<USkeletalMesh> weaponMesh(WeaponAssetPath);

		if (weaponMesh.Succeeded())
		{
			WeaponMesh->SetSkeletalMeshAsset(weaponMesh.Object);
		}

		// �̂̃��b�V���ɒǏ]
		WeaponMesh->SetupAttachment(GetMesh(), "hand_rSocket");
	}
}

void APlayerBase::SetupCollision()
{
	// ���̓����蔻��쐬
	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollision"));

	// ���̃��b�V���ɒǏ]
	WeaponCollision->SetupAttachment(WeaponMesh);
}



void APlayerBase::Move(const FInputActionValue& Value)
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

		// �ړ������ɉ�]
		RotateCharacter(MovementVector.Y);
	}
}

// �_�b�V���؂�ւ�
void APlayerBase::Run()
{
	// �R���g���[���\��
	if (!IsControl) { return; }

	if (!IsRunning)
	{
		// �_�b�V���I��
		IsRunning = true;
		CharacterMovementComp->MaxWalkSpeed = 600.f;
	}
	else
	{
		// �_�b�V���I��
		IsRunning = false;
		CharacterMovementComp->MaxWalkSpeed = 500.f;
	}

}

void APlayerBase::JumpStart()
{
	// �R���g���[���\��
	if (!IsControl) { return; }

	// �n��ɂ�����W�����v�J�n
	if (IsPlayerGrounded())
	{
		// �W�����v����
		frames = 0.f;

		JumpBeforePos_Z = GetActorLocation().Z;

		PlayerState = State_Jump;
	}
}

void APlayerBase::Jump()
{
	// ���݈ʒu
	FVector NowPos = GetActorLocation();

	// Sin�ō����X�V
	float SinValue = height * FMath::Sin(radPerFrame * frames);

	// �W�����v�O�̍�������ʒu�X�V
	const FVector nextPos(FVector(NowPos.X, NowPos.Y, SinValue + JumpBeforePos_Z));

	SetActorLocation(nextPos);

	// ���n���� ���~�J�n���画��J�n
	if (IsPlayerGrounded() && frames >= 20.f)
	{
		PlayerState = State_Idle;
	}

	// �t���[��+1
	frames += 1.f;
}

bool APlayerBase::IsPlayerGrounded() const
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
void APlayerBase::Attack(int AttackNum /*= 0*/)
{
	if (!IsAttacking)
	{
		// �U�����t���O�I��
		IsAttacking = true;
	}
	else
	{
		// �R���{�\�Ȏ�,�p��
		if (CanCombo)
		{
			// ���X�g�A�^�b�N�܂ŃR���{�p��
			if (ComboCntNames[ComboIndex] != ComboCntNames.Last())
			{
				++ComboIndex;
			}
		}
	}

	// �U���̃A�j���[�V�����Đ�
	PlayAnimation(ComboAnimMontages[AttackNum], ComboCntNames[ComboIndex]);
}

// �R���{1
void APlayerBase::Combo1()
{
	// �R���g���[���\��
	if (!IsControl) { return; }

	// �U��
	Attack(0);
}

// �R���{2
void APlayerBase::Combo2()
{
	// �R���g���[���\��
	if (!IsControl) { return; }

	// �U��
	Attack(1);
}

void APlayerBase::RotateCharacter(float nowInput_Y)
{
	// ���͂��Ȃ��ꍇ�͉������Ȃ�
	if (nowInput_Y == 0) { return; }

	// ��������
	FVector2D Direction;

	// ���͂̒l�ɉ����đO����������
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

	// �V���������ɃZ�b�g
	const FRotator nowRotate = FRotator(0.f, Direction.Y, Direction.X);

	SetActorRotation(nowRotate);
}


// �R���{�p��
void APlayerBase::ContinuationCombo()
{
	IsControl = true;
	CanCombo = true;
}


// �R���{���Z�b�g
void APlayerBase::ResetCombo()
{
	// �t���O���Z�b�g
	IsAttacking = false;
	CanCombo = false;
	IsControl = true;

	// �R���{���Z�b�g
	ComboIndex = 0;
}

void APlayerBase::SetCollision()
{
	// �R���W��������Ŗ������鍀�ڂ��w��(����͂���Actor�������g�Bthis�|�C���^�Ŏw��)
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	// �q�b�g����(=�R���W����������󂯂�)�I�u�W�F�N�g���i�[����ϐ�
	TArray<struct FHitResult> OutHits;


}

void APlayerBase::TakedDamage(float _damage)
{
	if (HP)
	{
		// HP�v�Z
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
	// �R���g���[���s�\��
	IsControl = false;

	// �Đ�����A�j���[�V�������i�[
	UAnimMontage* toPlayAnimMontage = ToPlayAnimMontage;

	// ���i�ڂ�
	FName StartSection = StartSectionName;

	// �A�j���[�V�����Đ�
	if (toPlayAnimMontage != nullptr)
	{
		PlayAnimMontage(toPlayAnimMontage, 1.f, StartSection);
	}
}