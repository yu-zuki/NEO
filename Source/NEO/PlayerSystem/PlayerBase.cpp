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

	// �X�v���C�����������Ċi�[
	AActor* tempSplineActor = GetSplineActor("PlayerLoad");

	// �A�N�^�[���X�v���C���ɃL���X�g
	if(tempSplineActor)
	{
		SplineActor = Cast<APlayerSpline>(tempSplineActor);
	}
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
	
	if (SplineActor)
	{
		// �X�v���C���̊p�x�擾
		const FRotator Rotation = SplineActor->GetSplineAngle(DistanceAdvanced * CharacterMovementComp->MaxWalkSpeed * deltaTime);

		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		// �ړ������擾(X,Y)
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		

		// �ړ�
		AddMovementInput(RightDirection, MovementVector.X);
		AddMovementInput(ForwardDirection, MovementVector.Y);

		// �ړ������ɉ�]
		RotateCharacter(MovementVector.X);

		// �ړ��ʕۑ�
		if (!ActionAssistComp->WallCheck())
		{
			DistanceAdvanced += MovementVector.X;

		}

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
		PlayerStatus.NowMoveSpeed = PlayerStatus.RunSpeed;
	}
	else
	{
		// �_�b�V���I��
		IsRunning = false;
		PlayerStatus.NowMoveSpeed = PlayerStatus.WalkSpeed;
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
	ActionAssistComp->CorrectAttackAngle();

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

	// �E�������Ă��邩�m�F
	bool LookRight = (_nowInput_X != 1.f) ? (true) : (false);

	// ��]
	ActionAssistComp->OwnerParallelToCamera(LookRight);

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

	// �Q�[�����[�h�쐬
	ATGS_GameMode* gameMode = Cast<ATGS_GameMode>(GetWorld()->GetAuthGameMode());
	UTGS_GameInstance* GameInstance = GetGameInstance();

	if (gameMode)
	{
		gameMode->DestroyPlayer(this);

		PlayerStatus.RemainingLife = GameInstance->LoadRemainingLife();

		// �c�@�����邤���̓��X�|�[��
		if (PlayerStatus.RemainingLife > 0)
		{
			// �v���C���[���X�|�[��
			gameMode->RespawnPlayer();

			// �c�@-�P
			GameInstance->SaveRemainingLife( --PlayerStatus.RemainingLife );
		}
		// ����ȊO�̓Q�[���I�[�o�[
		else
		{
			// �Q�[���I�[�o�[��
			gameMode->SetState_GameOver();

			// �v���C���[�폜
			gameMode->DestroyPlayer(this);
		}
	}
}

//�^�O����Actor���擾
AActor* APlayerBase::GetSplineActor(const FName _tag)
{
	//�Q�[���S�̂ɑ΂���Actor�̌����R�X�g���������߁A���ۑ����Ă��������ɂ���
	//�����Ώۂ͑S�Ă�Actor
	TSubclassOf<AActor> findClass;
	findClass = AActor::StaticClass();
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), findClass, actors);

	//�������ʁAActor�������
	if (actors.Num() > 0)
	{
		// ����Actor�̒������ԂɌ���
		for (int idx = 0; idx < actors.Num(); idx++)
		{
			AActor* pActor = Cast<AActor>(actors[idx]);

			// �^�O���Ŕ��f
			if (pActor->ActorHasTag(_tag))
			{
				//�f�o�b�O�m�F
				FString message = FString("Founded Actor	:") + pActor->GetName();
				UE_LOG(LogTemp, Warning, TEXT("%s"), *message);


				return pActor;
			}
		}
	}


	return NULL;
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
 * �֐����@�@�@�@�FTakedDamage()
 * �������e�@�@�@�F�v���C���[�̔�_���[�W����
 * �����P�@�@�@�@�Ffloat _damage�E�E�E��_���[�W��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void APlayerBase::TakedDamage(float _damage)
{
	if (PlayerStatus.HP >= 0.f)
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
			SetActorEnableCollision(true);

			// �q�b�g�G�t�F�N�g����
			ActionAssistComp->SpawnHitEffect(HitEffect, GetActorLocation());

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

			// �q�b�g�G�t�F�N�g����
			ActionAssistComp->SpawnHitEffect(HitEffect,GetActorLocation());

			// ��_���[�W�A�j���[�V�����Đ�
			PlayAnimation(PlayerAnimation.TakeDamage);
		}
	}
}


/*
 * �֐����@�@�@�@�FPlayAnimation()
 * �������e�@�@�@�F�v���C���[�̃A�j���[�V�����Đ�(�Đ����͑���s��)
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


