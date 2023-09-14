// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NEO/PlayerSystem/ActionAssistComponent.h"
#include "NEO/PlayerSystem/PlayerBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
	: IsHeld(false)
	, IsFalling(false)
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
	
	// �v���C���[�擾
	pPlayer = Cast<APlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DistanceCalculationToPlayer();

	if (IsFalling)
	{
		BlowsAway();
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
 * �����P�@�@�@�@�FACharacter* _owner�E�E�E�e�ɂȂ�L�����N�^�[
 * �����Q�@�@�@�@�FFName _socketName �E�E�E�\�P�b�g�̖��O
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::AttachToHand(ACharacter* _owner, FName _socketName,EOwnerType _ownerType)
{
	// ���ł���Ƃ��͎��Ȃ�
	if (IsFalling || IsHeld) { return; }

	// ������Ă����Ԃɂ���
	IsHeld = true;

	IsHoldDistance = false;

	// �I�[�i�[�ɐݒ�
	pOwner = _owner;

	// �I�[�i�[�̎�ނ�ݒ�
	OwnerType = _ownerType;
	
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
 * �֐����@�@�@�@�FDistanceCalculationToPlayer()
 * �������e�@�@�@�F�v���C���[�Ƃ̋����v�Z
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::DistanceCalculationToPlayer()
{
	if (IsHeld || IsFalling) { return; }

	APlayerBase* Player = Cast<APlayerBase>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (Player)
	{
		FVector PlayerPos = Player->GetActorLocation();
		// �v���C���[�Ƌ����v�Z
		float Distance = FVector::Dist(PlayerPos, GetActorLocation());


		if (Distance <= 120.f)
		{
			IsHoldDistance = true;

			if (Cast<APlayerBase>(Player)->GetPickUpWeapon() == nullptr)
			{
				Cast<APlayerBase>(Player)->SetPickUpWeapon(this);
				Cast<APlayerBase>(Player)->SetIsPickUpWeapon(true);
			}

		}
		else
		{
			IsHoldDistance = false;
			if (Cast<APlayerBase>(Player)->GetPickUpWeapon() == this)
			{
				Cast<APlayerBase>(Player)->SetPickUpWeapon(nullptr);
				Cast<APlayerBase>(Player)->SetIsPickUpWeapon(false);
			}
		}
	}

}


/*
 * �֐����@�@�@�@�FBlowsAway()
 * �������e�@�@�@�F���킪�������Œn�ʂɎh����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::BlowsAway()
{
	// ���݈ʒu
	FVector NowPos = GetActorLocation();

	// Sin�ō����X�V
	float SinValue = 150.f * FMath::Sin(radPerFrame * frames);

	// �W�����v�O�̍�������ʒu�X�V
	const FVector nextPos(FVector(NowPos.X, NowPos.Y, SinValue + FlyBeforePos.Z));

	SetActorLocation(nextPos);

	// �t���[��+1
	frames += 1.f;

	// ��]������
	FRotator NowRotation = GetActorRotation();

	// ��ɉ�]
	SetActorRotation(FRotator(NowRotation.Pitch, NowRotation.Yaw, NowRotation.Roll + 10.0));

	// ���̈ʒu���Ⴍ�Ȃ�����I��
	if (NowPos.Z < FlyBeforePos.Z)
	{
		FVector Pos = FVector::ZeroVector;
		
		switch (WeaponType)
		{
		case EWeaponType::WeaponType_Sword:
			break;
		case EWeaponType::WeaponType_Lance:
			Pos.Z = -70.f;
			break;
		case EWeaponType::WeaponType_Gun:
			break;
		case EWeaponType::WeaponType_None:
			break;
		default:
			break;
		}
		// �n�ʂɎh����悤�Ɉʒu�Ɗp�x��␳
		SetActorLocation(FlyBeforePos + Pos);
		SetActorRotation(DropAngle);

		// �t���[�����Z�b�g
		frames = 0.f;

		// ���ł����Ԃ̃t���O�����낷
		IsFalling = false;
	}
}


