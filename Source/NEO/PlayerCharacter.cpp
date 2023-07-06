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

	// �A�j���[�V�����Z�b�g�A�b�v
	SetupAnimationAsset();

	// ����̃Z�b�g�A�b�v
	SetupSword();

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

		// �W�����v
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::JumpStart);

		// �ړ�
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// �_�b�V��
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &APlayerCharacter::Run);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &APlayerCharacter::Run);

		// �R���{1
		EnhancedInputComponent->BindAction(ComboAction, ETriggerEvent::Started, this, &APlayerCharacter::Combo1);

		// �R���{2
		EnhancedInputComponent->BindAction(ComboAction2, ETriggerEvent::Started, this, &APlayerCharacter::Combo2);
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
	AssetPath.Add("/Game/Player/Input/Actions/IA_Combo1");
	AssetPath.Add("/Game/Player/Input/Actions/IA_Combo2");

	// ------------------����ver

	// �{�^���̃}�b�s���O�ݒ�
	DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/0122/Player/Input/IMC_Default"));

	// �e�A�N�V�����̃}�b�s���O
	MoveAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/0122/Player/Input/Actions/IA_Move"));
	RunAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/0122/Player/Input/Actions/IA_Dash"));
	JumpAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/0122/Player/Input/Actions/IA_Jump"));
	ComboAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/0122/Player/Input/Actions/IA_Combo1"));
	ComboAction2 = LoadObject<UInputAction>(nullptr, TEXT("/Game/0122/Player/Input/Actions/IA_Combo2"));

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

}

// �A�j���[�V�����̐ݒ�
void APlayerCharacter::SetupAnimationAsset()
{
	//�A�j���[�V�����A�Z�b�g���������Ċi�[
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

	// �R���{�̖��O�i�[
	ComboCntNames = { "First", "Second", "Third"/*,"Fourth"*/};
}

void APlayerCharacter::SetupSword()
{
	// ���̃R���|�[�l���g���쐬
	SwordMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

	// ���̃A�Z�b�g�ݒ�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> swordMesh(TEXT("/Game/0122/Player/Weapon/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight"));

	if (swordMesh.Succeeded())
	{
		SwordMesh->SetSkeletalMeshAsset(swordMesh.Object);
	}

	// �̂̃��b�V���ɒǏ]
	SwordMesh->SetupAttachment(GetMesh(),"hand_rSocket");

	// ���̓����蔻��쐬
	SwordCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollision"));

	// ���̃��b�V���ɒǏ]
	SwordCollision->SetupAttachment(SwordMesh);
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

		// ��������
		float Direction;

		// �����w��
		if (MovementVector.Y)
		{
			Direction = 90.f;
		}
		else if (MovementVector.Y == 0)
		{
			Direction = Rotation.Pitch;
		}
		else
		{
			Direction = -90.f;
		}

		// �Z�b�g
		SetActorRotation(FRotator(0.f, Direction, 0.f));
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
	
	// ���n���� �W�����v�J�n���琔�t���[���ォ�画��J�n
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
void APlayerCharacter::Attack(int ComboNum /*= 0*/)
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
	PlayAnimation(ComboAnimMontages[ComboNum], ComboCntNames[ComboIndex]);
}

// �R���{1
void APlayerCharacter::Combo1()
{
	// �R���g���[���\��
	if (!IsControl) { return; }

	// �U��
	Attack(0);
}

// �R���{2
void APlayerCharacter::Combo2()
{
	// �R���g���[���\��
	if (!IsControl) { return; }

	// �U��
	Attack(1);
}

// �R���{�p��
void APlayerCharacter::ContinuationCombo()
{
	IsControl = true;
	CanCombo = true;
}


// �R���{���Z�b�g
void APlayerCharacter::ResetCombo()
{
	// �t���O���Z�b�g
	IsAttacking = false;
	CanCombo = false;
	IsControl = true;

	// �R���{���Z�b�g
	ComboIndex = 0;
}

void APlayerCharacter::SetSwordCollision()
{
	// �R���W��������Ŗ������鍀�ڂ��w��(����͂���Actor�������g�Bthis�|�C���^�Ŏw��)
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	// �q�b�g����(=�R���W����������󂯂�)�I�u�W�F�N�g���i�[����ϐ�
	TArray<struct FHitResult> OutHits;


}

void APlayerCharacter::TakedDamage(float _damage)
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

void APlayerCharacter::PlayAnimation(UAnimMontage* ToPlayAnimMontage, FName StartSectionName /*= "None"*/)
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