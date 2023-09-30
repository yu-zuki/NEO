// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NEO/PlayerSystem/ActionAssistComponent.h"
#include "NEO/PlayerSystem/PlayerBase.h"
#include "NEO/PlayerSystem/NEOPlayerController.h"
#include "NEO/BackGroundSystem/ObjectBase.h"
#include "NEO/Enemy/EnamyBase.h"
#include "NEO/OdaBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
	: CalculateInterval_nearPlayer(1.f)
	, CalculateInterval_farPlayer(10.f)
	, DistanceToOmitCalc(2000.f)
	, JumpHeight(150.f)
	, IsHeld(false)
	, IsFalling(false)
	, Frames(0.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	// �^�O�ݒ�
	Tags.Add("Weapon");

	// �A�^�b�N�A�V�X�g�R���|�[�l���g�쐬
	ActionAssistComp = CreateDefaultSubobject<UActionAssistComponent>(TEXT("AttackAssist"));
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	// �R���g���[���[�擾
	PlayerController = Cast<ANEOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsFalling)
	{
		BlowsAway();
	}
}

/*
 * �֐����@�@�@�@�FSetupWeaponMesh()
 * �������e�@�@�@�F�v���C���[�̃��b�V�����Z�b�g�A�b�v
 * �����P�@�@�@�@�FUStaticMeshComponent*& MeshComp�E�E�E���b�V���R���|�[�l���g
 * �����Q�@�@�@�@�FTCHAR* WeaponAssetPath �E�E�E�E�E�E�E����̃A�Z�b�g�̃p�X
 * �����R�@�@�@�@�FFName PublicName �E�E�E�E�E�E�E�E�E�E�G�f�B�^�ł̌��J��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::SetupWeaponMesh(UStaticMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName /*= "WeaponMesh"*/)
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
	}
}


/*
 * �֐����@�@�@�@�FAttachToHand()
 * �������e�@�@�@�F�L�����N�^�[�ɂ�������
 * �����P�@�@�@�@�FACharacter* _owner �E�E�E�E�e�ɂȂ�L�����N�^�[
 * �����Q�@�@�@�@�FFName _socketName�E�E�E�E�E�\�P�b�g�̖��O
 * �����R		 �FEOwnerType _ownerType�E�E�E�I�[�i�[�̎��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::AttachToHand(ACharacter* _owner, FName _socketName,EOwnerType _ownerType)
{
	// ���ł���Ƃ��͎��Ȃ�
	if (IsFalling || IsHeld) { return; }

	// ������Ă����Ԃɂ���
	IsHeld = true;

	// �I�[�i�[�̎�ނ�ݒ�
	OwnerType = _ownerType;

	// �I�[�i�[�̃N���X�擾
	pOwner = _owner;

	// �R���g���[���[����폜
	PlayerController->RemoveWeapons(this);

	
	// �L�����N�^�[�ɃA�^�b�`
	if (pOwner)
	{
		AttachToComponent(pOwner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false), _socketName);
	}
}


/*
 * �֐����@�@�@�@�FDetachToHand()
 * �������e�@�@�@�F�L�����N�^�[�ɂ�������
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::DetachToHand()
{
	// ������Ă��Ȃ���Ԃɂ���
	IsHeld = false;

	// �L�����N�^�[����O��
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld,false));

	// �I�[�i�[�����Ȃ��Ȃ�
	pOwner = nullptr;

	// ������΂��t���O�����Ă�
	IsFalling = true;

	// ��ԑO�̏ꏊ�ݒ�
	FlyBeforePos = GetActorLocation();
}


/*
 * �֐����@�@�@�@�FGetHitResults()
 * �������e�@�@�@�F�U���������������ǂ�������
 * �����P�@�@�@�@�FFVector _start �E�E�E�E�E�E�E�E �E�E�E�E�����蔻����Ƃ�͈�(�J�n�n�_)
 * �����Q�@�@�@�@�FFVector _end�E�E�E�E�E�E�E�E�E�E�E�E�E�E�����蔻����Ƃ�͈�(�I���n�_)
 * �����R		 �FTArray<FHitResult>& _outHitResults�E�E�E���������I�u�W�F�N�g
 * �߂�l�@�@�@�@�F�����������ǂ���
 */
bool AWeaponBase::GetHitResults(FVector _start, FVector _end, TArray<FHitResult>& _outHitResults)
{
	// ����{�̂ƃI�[�i�[�ɂ͓�����Ȃ��悤�ɂ���
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(pOwner);

	// ���������������擾
	FQuat Rot = WeaponCollision->GetComponentQuat();
	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(WeaponCollision->GetScaledCapsuleRadius(), WeaponCollision->GetScaledCapsuleHalfHeight());

	// �������Ă��邩�m�F
	bool IsHit = GetWorld()->SweepMultiByChannel(_outHitResults, _start, _end, Rot, ECollisionChannel::ECC_GameTraceChannel1, CollisionShape, CollisionParams);

	return IsHit;
}


/*
 * �֐����@�@�@�@�FAttackObject()
 * �������e�@�@�@�F�j��\�I�u�W�F�N�g�ɍU���������̏���
 * �����P�@�@�@�@�FAActor* _object �E�E�E�E�E�E�E���������I�u�W�F�N�g
 * �����Q�@�@�@�@�Ffloat _damageAmount �E�E�E�E�E�_���[�W��
 * �����R		 �FUSoundBase* _hitSoundObj�E�E�E�q�b�g���ɂȂ�T�E���h
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::AttackObject(AActor* _object, float _damageAmount, USoundBase* _hitSoundObj)
{
	AObjectBase* Object = Cast<AObjectBase>(_object);

	if (Object)
	{
		// �_���[�W��^����
		Object->ReceiveDamage(_damageAmount);

		// �I�u�W�F�N�g�j��p�̃T�E���h�Đ�
		ActionAssistComp->PlaySound(_hitSoundObj);
	}
}


/*
 * �֐����@�@�@�@�FAttackEnemy()
 * �������e�@�@�@�F�G�ɍU���������̏���
 * �����P�@�@�@�@�FAActor* _enemy�E�E�E�E�E�E�E�E���������G
 * �����Q�@�@�@�@�Ffloat _damageAmount �E�E�E�E�E�_���[�W��
 * �����R		 �FUSoundBase* _hitSoundObj�E�E�E�q�b�g���ɂȂ�T�E���h
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::AttackEnemy(AActor* _enemy, float _damageAmount, USoundBase* _hitSoundObj)
{
	// �v���C���[�擾
	APlayerBase* Player = Cast<APlayerBase>(pOwner);

	// �G�l�~�[��damage����
	AEnamyBase* Enemy = Cast<AEnamyBase>(_enemy);
	AOdaBase* Boss = Cast<AOdaBase>(_enemy);

	// �R���{�擾
	int ComboNum = Player->GetComboIndex();

	// �q�b�g�X�g�b�v�̎��Ԃ��擾
	const float HitStopTime = SetHitStopTime(ComboNum);

	// �q�b�g�X�g�b�v
	Player->HitStop(0.1f, HitStopTime);


	if (Enemy)
	{
		// �G�̃_���[�W����
		Enemy->ApplyDamage(_damageAmount);

		// �G�̃m�b�N�o�b�N����
		EnemyKnockBack(ComboNum,Enemy);

		// �G����ɓ����Ă���̂�
		// �R���{��������₷���悤�ɓG�ɂ��~�܂��Ă��炤
		Enemy->ActionAssistComp->HitStop(0.1f, HitStopTime);
	}
	else if (Boss)
	{
		// �{�X�̃_���[�W����
		Boss->ApplyDamage(_damageAmount);

		// �R���{���ŏI�i�̎��m�b�N�o�b�N
		if (Player->GetComboIndex() == 3)
		{
			Boss->BossKnockback();
		}
	}

	// �R���{�̃t�B�j�b�V���̂݃J������h�炷
	if (ComboNum == 3)
	{
		Player->CameraShake();
	}

	if (_hitSoundObj)
	{
		// �a��SE�Đ�
		ActionAssistComp->PlaySound(_hitSoundObj);
	}
}


/*
 * �֐����@�@�@�@�FEnemyKnockBack()
 * �������e�@�@�@�F�G�̃m�b�N�o�b�N����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::EnemyKnockBack(int _comboNum, AEnamyBase* _enemy)
{
	if (!_enemy) { return; }

	// �m�b�N�o�b�N�̋����i�[�p
	float KnockBackDistance = 0.f;

	// �R���{�ɂ���ăm�b�N�o�b�N
	if (_comboNum == 1)
	{
		KnockBackDistance = -100.f;
	}
	else if (_comboNum == 2)
	{
		KnockBackDistance = -150.f;
	}
	else if (_comboNum == 3)
	{
		KnockBackDistance = -250.f;
	}

	// �m�b�N�o�b�N
	if (!_enemy->ActionAssistComp->WallCheck(KnockBackDistance))
	{
		_enemy->AddActorLocalOffset(FVector(KnockBackDistance, 0.f, 0.f));
	}
}


/*
 * �֐����@�@�@�@�FDistanceCalculationToPlayer()
 * �������e�@�@�@�F�v���C���[�Ƃ̋����v�Z
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::DistanceCalculationToPlayer()
{
	if (IsHeld || IsFalling) { return; }

	if (!PlayerController) { return; }

	// �v���C���[�̈ʒu�擾
	FVector PlayerPos = PlayerController->GetPlayerLocation();

	// �v���C���[�Ƌ����v�Z
	DistanceToPlayer = FVector::Dist(PlayerPos, GetActorLocation());

	// �E���鋗����������
	if (DistanceToPlayer <= PlayerController->GetPickUpDistance())
	{
		if (!IsPickUpDistanceEnteredPlayer)
		{
			// �R���g���[���[�ɒǉ�
			PlayerController->AddWeapons(this);

			// ��苗�����Ƀv���C���[��������
			IsPickUpDistanceEnteredPlayer = true;
		}
	}
	else
	{
		if (IsPickUpDistanceEnteredPlayer)
		{
			// �R���g���[���[����폜
			PlayerController->RemoveWeapons(this);

			// ��苗���O�Ƀv���C���[���ł�
			IsPickUpDistanceEnteredPlayer = false;
		}
	}

	// �v���C���[�Ƃ̋����ɂ���Čv�Z�̊Ԋu�w��
	const float CalculateInterval = (DistanceToPlayer > DistanceToOmitCalc) ? (CalculateInterval_farPlayer) : (CalculateInterval_nearPlayer);

	// ��莞�Ԍ�ɍēx�������v�Z
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(TimerHandle, this, &AWeaponBase::DistanceCalculationToPlayer, CalculateInterval, false);
}


/*
 * �֐����@�@�@�@�FBlowsAway()
 * �������e�@�@�@�F���킪�����Ēn�ʂɎh����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::BlowsAway()
{
	// ���݈ʒu
	FVector NowPos = GetActorLocation();

	// Sin�ō����X�V
	float SinValue = JumpHeight * FMath::Sin(RadPerFrame * Frames);

	// �W�����v�O�̍�������ʒu�X�V
	const FVector nextPos(FVector(NowPos.X, NowPos.Y, SinValue + FlyBeforePos.Z));
	SetActorLocation(nextPos);

	// �t���[���X�V
	Frames += 1.f;

	// ���݂̉�]�擾
	FRotator NowRotation = GetActorRotation();

	// ��ɉ�]
	SetActorRotation(FRotator(NowRotation.Pitch, NowRotation.Yaw, NowRotation.Roll + 10.0));

	// ���̈ʒu���Ⴍ�Ȃ�����I��
	if (NowPos.Z < FlyBeforePos.Z)
	{
		// ���ł����Ԃ̃t���O�����낷
		IsFalling = false;

		// �t���[�����Z�b�g
		Frames = 0.f;

		// �������Đ�
		if (DropWeaponSoundObj)
		{
			// �a��SE�Đ�
			ActionAssistComp->PlaySound(DropWeaponSoundObj);
		}

		// �n�ʂɎh����悤�Ɉʒu�Ɗp�x��␳
		SetActorLocation(FlyBeforePos + DropLocation);
		SetActorRotation(DropAngle);

		// �v���C���[�R���g���[���[�ɏE����悤�ɂȂ������Ƃ�`����
		PlayerController->AddWeapons(this);

		//�v���C���[�Ƃ̌v�Z�������J�n
		DistanceCalculationToPlayer();
	}
}


