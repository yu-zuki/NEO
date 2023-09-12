// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
	: IsHeld(true)
	, IsFalling(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	

	// �^�O�ݒ�
	Tags.Add("Weapon");
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

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
	if (IsFalling) { return; }

	// ������Ă����Ԃɂ���
	IsHeld = true;

	// �I�[�i�[�ɐݒ�
	OwnerInfo.pOwner = _owner;

	// �I�[�i�[�̎�ނ�ݒ�
	OwnerType = _ownerType;
	
	// �L�����N�^�[�ɃA�^�b�`
	if (OwnerInfo.pOwner)
	{
		AttachToComponent(OwnerInfo.pOwner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false), _socketName);
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
	OwnerInfo.pOwner = nullptr;

	// ������΂��t���O�����Ă�
	IsFalling = true;

	// ��ԑO�̏ꏊ�ݒ�
	FlyBeforePos = GetActorLocation();

	// Tick�����ĊJ
	PrimaryActorTick.bCanEverTick = true;
}

/*
 * �֐����@�@�@�@�FSetupOwnerData()
 * �����P�@�@�@�@�FACharacter* _owner�E�E�E�e�ɂȂ�L�����N�^�[
 * �����Q�@�@�@�@�FFName _ownerTag �E�E�E�E�I�[�i�[�ɂ��Ă���^�O
 * �����R�@�@�@�@�FFName _socketName �E�E�E�\�P�b�g�̖��O
 * �������e�@�@�@�F�I�[�i�[�̏�񏉊���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::SetupOwnerData(ACharacter* _owner, FName _ownerTag, FName _socketName)
{
	if (_owner)
	{
		OwnerInfo.pOwner = _owner;
		OwnerInfo.OwnerTag = _ownerTag;
		OwnerInfo.SocketName = _socketName;
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
		// �n�ʂɎh����悤�Ɉʒu�Ɗp�x��␳
		SetActorLocation(FlyBeforePos);
		SetActorRotation(FRotator(0.f, 0.f, -90.f));

		// �t���[�����Z�b�g
		frames = 0.f;

		// Tick�������f
		PrimaryActorTick.bCanEverTick = false;

		// ���ł����Ԃ̃t���O�����낷
		IsFalling = false;
	}
}


