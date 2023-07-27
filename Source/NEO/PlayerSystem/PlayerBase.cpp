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


#define DIRECTION_X (25.f)
#define DIRECTION_Y (90.f)

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

	// �v���C���[�̐ݒ�
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// �v���C���[�ɉ�]�̐���
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// �^�O�ݒ�
	Tags.Add("Player");

	// �L�����N�^�[�R���|�[�l���g�擾
	CharacterMovementComp = GetCharacterMovement();
	CharacterMovementComp->MaxWalkSpeed = 500.f;

	// �A�^�b�N�A�V�X�g�R���|�[�l���g�쐬
	AttackAssistComp = CreateDefaultSubobject<UAttackAssistComponent>(TEXT("AttackAssist"));
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

	// �����p�x�ݒ�
	const FRotator nowRotate = FRotator(0.f, DIRECTION_Y, -DIRECTION_X);
	SetActorRotation(nowRotate);
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
		EnhancedInputComponent->BindAction(MainActionMapping.MoveAction, ETriggerEvent::Triggered, this, &APlayerBase::Move);

		// ����							
		EnhancedInputComponent->BindAction(MainActionMapping.RunAction, ETriggerEvent::Started, this, &APlayerBase::Run);
		EnhancedInputComponent->BindAction(MainActionMapping.RunAction, ETriggerEvent::Completed, this, &APlayerBase::Run);

		// �W�����v
		EnhancedInputComponent->BindAction(MainActionMapping.JumpAction, ETriggerEvent::Started, this, &APlayerBase::JumpStart);

		// �R���{�A�N�V����
		EnhancedInputComponent->BindAction(MainActionMapping.ComboAction1, ETriggerEvent::Started, this, &APlayerBase::Combo1);
		EnhancedInputComponent->BindAction(MainActionMapping.ComboAction2, ETriggerEvent::Started, this, &APlayerBase::Combo2);
	}
}


/*
 * �֐����@�@�@�@�FSetupPlayerData()
 * �������e�@�@�@�F�v���C���[�̃f�[�^������
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::SetupPlayerData()
{
	// ���C���A�N�V�����̃{�^�����}�b�s���O
	SetupMainActionMapping();

	// �R���{�̖��O�i�[
	ComboStartSectionNames = { "First", "Second", "Third"/*,"Fourth"*/ };
}


/*
 * �֐����@�@�@�@�FSetupPlayerStatus()
 * �������e�@�@�@�F�v���C���[�̃X�e�[�^�X������
 * �����P�@�@�@�@�Ffloat _hp�E�E�E�E�E�E�E�E�EHP�̏����l
 * �����Q		 �Fint _remainingLife �E�E�E�E������
 * �����R�@�@�@�@�Ffloat _damageAmount�E�E�E�E�U���͂̏����l
 * �����S�@�@�@�@�Ffloat _jumpHeight�E�E�E�E�E�W�����v�͂̏����l
 * �����T�@�@�@�@�Ffloat _comboDamageFactor�E�R���{���Ƃ̃_���[�W�̔{��
 * �����U�@�@�@�@�Ffloat _walkSpeed�E�E�E�E�E�������x
 * �����V�@�@�@�@�Ffloat _runSpeed �E�E�E�E�E���鑬�x
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::SetupPlayerStatus(float _hp /*= 100.f*/, int _remainingLife /*= 3.f*/, float _damageAmount /*= 10.f*/,
									float _jumpHeight /*= 150.f*/, float _comboDamageFactor /*= 1.f*/, float _walkSpeed /*= 500.f*/, float _runSpeed /*= 600.f*/)
{
	PlayerStatus.HP = _hp;
	PlayerStatus.MaxHP = _hp;
	PlayerStatus.RemainingLife = _remainingLife;
	PlayerStatus.DamageAmount = _damageAmount;
	PlayerStatus.JumpHeight = _jumpHeight;
	PlayerStatus.ComboDamageFactor = _comboDamageFactor;
	PlayerStatus.WalkSpeed = _walkSpeed;
	PlayerStatus.RunSpeed = _runSpeed;
}	

/*
 * �֐����@�@�@�@�FSetupMainActionMapping()
 * �������e�@�@�@�F�v���C���[�̓��̓}�b�s���O
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::SetupMainActionMapping()
{
	// �{�^���ݒ�
	TCHAR* defaultMappingContext = TEXT("/Game/0122/Player/Input/IMC_MainAction");

	// ���ꂼ��̃A�N�V�����̃p�X
	TArray<TCHAR*> inputActionPaths;
	inputActionPaths.Add(TEXT("/Game/0122/Player/Input/Actions/IA_Move"));
	inputActionPaths.Add(TEXT("/Game/0122/Player/Input/Actions/IA_Dash"));
	inputActionPaths.Add(TEXT("/Game/0122/Player/Input/Actions/IA_Jump"));
	inputActionPaths.Add(TEXT("/Game/0122/Player/Input/Actions/IA_Combo1"));
	inputActionPaths.Add(TEXT("/Game/0122/Player/Input/Actions/IA_Combo2"));

	// �{�^���̃}�b�s���O�ݒ�
	MainActionMapping.DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, defaultMappingContext);

	// �e�A�N�V�����̃}�b�s���O
	for (int i = 0; i < inputActionPaths.Num(); ++i)
	{
		// �ꎞ�ۑ��p
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
 * �֐����@�@�@�@�FSetupWeaponMesh()
 * �������e�@�@�@�F�v���C���[�̃��b�V�����Z�b�g�A�b�v(������StaticMesh�̏ꍇ)
 * �����P�@�@�@�@�FUStaticMeshComponent*& MeshComp�E�E�E���b�V���R���|�[�l���g
 * �����Q�@�@�@�@�FTCHAR* WeaponAssetPath �E�E�E�E�E�E�E����̃A�Z�b�g�̃p�X
 * �����R�@�@�@�@�FFName PublicName �E�E�E�E�E�E�E�E�E�E�G�f�B�^�ł̌��J��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::SetupWeaponMesh(UStaticMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName /*= "WeaponMesh"*/)
{
	// ����̃R���|�[�l���g���쐬
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(PublicName);

	if (WeaponAssetPath)
	{
		// ����̃A�Z�b�g�ݒ�
		ConstructorHelpers::FObjectFinder< UStaticMesh > weaponMesh(WeaponAssetPath);

		if (weaponMesh.Succeeded())
		{
			MeshComp->SetStaticMesh(weaponMesh.Object);
		}

		// �̂̃��b�V���ɒǏ]
		MeshComp->SetupAttachment(GetMesh(), "hand_rSocket");
	}
}


/*
 * �֐����@�@�@�@�FSetupWeaponMesh()
 * �������e�@�@�@�F�v���C���[�̃��b�V�����Z�b�g�A�b�v(������SkeletalMesh�̏ꍇ)
 * �����P�@�@�@�@�FUSkeletalMeshComponent*& MeshComp�E�E�E���b�V���R���|�[�l���g
 * �����Q�@�@�@�@�FTCHAR* WeaponAssetPath �E�E�E�E�E�E�E�E����̃A�Z�b�g�̃p�X
 * �����R�@�@�@�@�FFName PublicName �E�E�E�E�E�E�E�E�E�E�E�G�f�B�^�ł̌��J��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::SetupWeaponMesh(USkeletalMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName /*= "WeaponMesh"*/)
{
	// ����̃R���|�[�l���g���쐬
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(PublicName);

	if (WeaponAssetPath)
	{
		// ����̃A�Z�b�g�ݒ�
		ConstructorHelpers::FObjectFinder< USkeletalMesh > weaponMesh(WeaponAssetPath);

		if (weaponMesh.Succeeded())
		{
			MeshComp->SetSkeletalMeshAsset(weaponMesh.Object);
		}

		// �̂̃��b�V���ɒǏ]
		MeshComp->SetupAttachment(GetMesh(), "hand_rSocket");
	}
}


/*
 * �֐����@�@�@�@�FGetAnimationAsset()
 * �������e�@�@�@�F�A�j���[�V�����A�Z�b�g��Ԃ�
 * �����P�@�@�@�@�FTCHAR* _animAssetPath �E�E�E�A�j���[�V�����A�Z�b�g�̃p�X
 * �߂�l�@�@�@�@�F���������A�j���[�V����
 */
UAnimMontage* APlayerBase::GetAnimationAsset(TCHAR* _animAssetPath)
{
	// �A�Z�b�g��T���ăZ�b�g
	ConstructorHelpers::FObjectFinder<UAnimMontage> SearchAnimMontage(_animAssetPath);

	if (SearchAnimMontage.Succeeded())
	{
		UAnimMontage* FoundAnimMontage = SearchAnimMontage.Object;

		return FoundAnimMontage;
	}

	return nullptr;
}


/*
 * �֐����@�@�@�@�FMove()
 * �������e�@�@�@�F�v���C���[�̓��͎�t(�ړ�����)
 * �����P�@�@�@�@�FFInputActionValue& Value�E�E�E���͗�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::Move(const FInputActionValue& _value)
{
	// �R���g���[���\��
	if (!IsControl) { return; }

	// input is a Vector2D
	FVector2D MovementVector = _value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		// �ړ������擾(X,Y)
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// �ړ�
		AddMovementInput(RightDirection, MovementVector.X);
		AddMovementInput(ForwardDirection, MovementVector.Y);

		// �ړ������ɉ�]
		RotateCharacter(MovementVector.X);
	}
}


/*
 * �֐����@�@�@�@�FRun()
 * �������e�@�@�@�F�v���C���[�̓��͎�t(�_�b�V���؂�ւ�)
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::Run()
{
	// �R���g���[���\��
	if (!IsControl) { return; }

	if (!IsRunning)
	{
		// �_�b�V���I��
		IsRunning = true;
		CharacterMovementComp->MaxWalkSpeed = PlayerStatus.RunSpeed;
	}
	else
	{
		// �_�b�V���I��
		IsRunning = false;
		CharacterMovementComp->MaxWalkSpeed = PlayerStatus.WalkSpeed;
	}
}


/*
 * �֐����@�@�@�@�FJumpStart()
 * �������e�@�@�@�F�v���C���[�̓��͎�t(�W�����v�J�n)
 * �߂�l�@�@�@�@�F�Ȃ�
 */
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


/*
 * �֐����@�@�@�@�FJump()
 * �������e�@�@�@�F�v���C���[�̃W�����v������
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::Jump()
{
	// ���݈ʒu
	FVector NowPos = GetActorLocation();

	// Sin�ō����X�V
	float SinValue = PlayerStatus.JumpHeight * FMath::Sin(radPerFrame * frames);

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


/*
 * �֐����@�@�@�@�FIsPlayerGrounded()
 * �������e�@�@�@�F�v���C���[���n�ʂɂ��Ă��邩����
 * �߂�l�@�@�@�@�F�n�ʂɂ��Ă�����true
 */
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


/*
 * �֐����@�@�@�@�FAttack()
 * �������e�@�@�@�F�v���C���[�̍U������
 * �����P�@�@�@�@�Fint _attackNum�E�E�E�U���A�j���[�V�����̎�ޔ��ʗp
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::Attack(int _attackNum /*= 0*/)
{
	// �v���C���[�̊p�x�C��
	AttackAssistComp->CorrectAttackAngle();

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
			if (ComboStartSectionNames[ComboIndex] != ComboStartSectionNames.Last())
			{
				++ComboIndex;
			}
		}
	}

	// �U���̃A�j���[�V�����Đ�
	PlayAnimation(PlayerAnimation.Combo[_attackNum],ComboStartSectionNames[ComboIndex]);
}


/*
 * �֐����@�@�@�@�FCombo1()
 * �������e�@�@�@�F�v���C���[�̓��͎�t(�U���P��)
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::Combo1()
{
	// �R���g���[���\��
	if (!IsControl) { return; }

	// �U��
	Attack(0);
}

/*
 * �֐����@�@�@�@�FCombo2()
 * �������e�@�@�@�F�v���C���[�̓��͎�t(�U���Q��)
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::Combo2()
{
	// �R���g���[���\��
	if (!IsControl) { return; }

	// �U��
	Attack(1);
}


/*
 * �֐����@�@�@�@�FRotateCharacter()
 * �������e�@�@�@�F�v���C���[�̃X�e�[�^�X������
 * �����P�@�@�@�@�Ffloat _nowInput_X�E�E�E���݂̈ړ����͒l
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::RotateCharacter(float _nowInput_X)
{
	// ���͂��Ȃ��ꍇ�͉������Ȃ�
	if (_nowInput_X == 0) { return; }

	// ��������
	FVector2D Direction;

	// ���͂̒l�ɉ����đO����������
	if (_nowInput_X == 1.f)
	{
		Direction.X = -DIRECTION_X;
		Direction.Y = DIRECTION_Y;
	}
	else
	{
		Direction.X = DIRECTION_X;
		Direction.Y = -DIRECTION_Y;
	}

	// �V���������ɃZ�b�g
	const FRotator nowRotate = FRotator(0.f, Direction.Y, Direction.X);

	SetActorRotation(nowRotate);
}


/*
 * �֐����@�@�@�@�FSlowDawnDeathAnimationRate()
 * �������e�@�@�@�F���S���A�j���[�V���������L�΂�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::SlowDownDeathAnimationRate()
{
	// �x������
	GetMesh()->GlobalAnimRateScale = DeadAnimRate;


	// �v���C���[���폜
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(TimerHandle_DeathToGameOver, this, &APlayerBase::CallGameModeFunc_DestroyPlayer, DeadToGameOverTime, false);
}


/*
 * �֐����@�@�@�@�FCallGameModeFunc_DestroyPlayer()
 * �������e�@�@�@�F���S���̃Q�[�����[�h���̊֐��Ăяo��
 * �@�@�@�@�@�@�@�@�c�@���c���Ă��烊�X�|�[��,�Ȃ��Ȃ�����v���C���[�폜
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::CallGameModeFunc_DestroyPlayer()
{
	//// �Q�[�����[�h�쐬
	//ATGS_GameMode* gameMode = Cast<ATGS_GameMode>(GetWorld()->GetAuthGameMode());

	//if (gameMode)
	//{
	//	// �c�@�����邤���̓��X�|�[��
	//	if (PlayerStatus.RemainingLife > 0)
	//	{
	//		// �v���C���[���X�|�[��
	//		gameMode->RespawnPlayer();

	//		// �c�@-�P
	//		--PlayerStatus.RemainingLife;
	//	}
	//	// ����ȊO�̓Q�[���I�[�o�[
	//	else
	//	{
	//		// �Q�[���I�[�o�[��
	//		gameMode->SetState_GameOver();

	//		// �v���C���[�폜
	//		gameMode->DestroyPlayer(this);
	//	}
	//}
}



/*
 * �֐����@�@�@�@�FContinuationCombo()
 * �������e�@�@�@�F�R���{�̌p��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::ContinuationCombo()
{
	IsControl = true;
	CanCombo = true;
}


/*
 * �֐����@�@�@�@�FResetCombo()
 * �������e�@�@�@�F�R���{���Z�b�g
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::ResetCombo()
{
	// �t���O���Z�b�g
	IsAttacking = false;
	CanCombo = false;
	IsControl = true;

	// �R���{���Z�b�g
	ComboIndex = 0;
}

/*
 * �֐����@�@�@�@�FSetCollision()
 * �������e�@�@�@�F�v���C���[�̃R���W��������(�I�[�o�[���C�h�p)
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::SetCollision()
{
	return;
}


/*
 * �֐����@�@�@�@�FTakedDamage()
 * �������e�@�@�@�F�v���C���[�̔�_���[�W����
 * �����P�@�@�@�@�Ffloat _damage�E�E�E��_���[�W��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::TakedDamage(float _damage)
{
	if (PlayerStatus.HP)
	{
		// HP�v�Z
		PlayerStatus.HP -= _damage;

		// HP��0�ȉ��ɂȂ�����
		if (PlayerStatus.HP <= 0.f)
		{
			// HP��0�ȉ��Ȃ玀
			PlayerState = State_Death;

			IsControl = false;

			// ���S�A�j���[�V�����Đ�
			PlayAnimation(PlayerAnimation.Death);
		}
		else
		{
			// �U�����̃t���O���Z�b�g
			if (IsAttacking)
			{
				IsAttacking = false;
				CanCombo = false;
				IsControl = true;
				ComboIndex = 0;
			}

			// ��_���[�W�A�j���[�V�����Đ�
			PlayAnimation(PlayerAnimation.TakeDamage);
		}
	}


}


/*
 * �֐����@�@�@�@�FPlayAnimation()
 * �������e�@�@�@�F�v���C���[�̃A�j���[�V�����Đ�(�Đ����͑���s��)
 * �����P�@�@�@�@�FUAnimMontage* _toPlayAnimMontage �E�E�E�Đ�����A�j���[�V����
 * �����Q�@�@�@�@�FFName _startSectionName�E�E�E�E�E�E�E�E�R���{�̉��i�ڂ���Đ����邩
 * �����R�@�@�@�@�Ffloat _playRate�E�E�E�E�E�E�E�E�E�E�E�E�A�j���[�V�����̍Đ����x
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::PlayAnimation(UAnimMontage* _toPlayAnimMontage, FName _startSectionName /*= "None"*/, float _playRate /*= 1.f*/)
{
	// �R���g���[���s�\��
	IsControl = false;

	// �Đ�����A�j���[�V�������i�[
	UAnimMontage* toPlayAnimMontage = _toPlayAnimMontage;

	// �A�j���[�V�����Đ�
	if (toPlayAnimMontage != nullptr)
	{
		PlayAnimMontage(_toPlayAnimMontage, _playRate, _startSectionName);
	}
}