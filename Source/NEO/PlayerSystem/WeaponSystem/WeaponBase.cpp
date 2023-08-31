// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	//pPlayer = UGameplayStatics::GetPlayerCharacter(this, 0);
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// �ڐG�J�n���ɍs������
void AWeaponBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �I�[�o�[���b�v�����ۂɎ��s�������C�x���g
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		// �I�[�o�[���b�v�����̂��v���C���[�̎��̂ݔ�����������
		// PlayerCharaBP -> Actor -> Tags�ɐݒ肵���^�O�uPlayer�v�ŁA�v���C���[�����ʂ���
		if (OtherActor->ActorHasTag("Player"))
		{
			//// �f�o�b�O�m�F�p
			// UE_LOG(LogTemp,Warning,TEXT("Goal"));



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
 * �֐����@�@�@�@�FSetupWeaponMesh()
 * �������e�@�@�@�F�v���C���[�̃��b�V�����Z�b�g�A�b�v(������SkeletalMesh�̏ꍇ)
 * �����P�@�@�@�@�FUSkeletalMeshComponent*& MeshComp�E�E�E���b�V���R���|�[�l���g
 * �����Q�@�@�@�@�FTCHAR* WeaponAssetPath �E�E�E�E�E�E�E�E����̃A�Z�b�g�̃p�X
 * �����R�@�@�@�@�FFName PublicName �E�E�E�E�E�E�E�E�E�E�E�G�f�B�^�ł̌��J��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::SetupWeaponMesh(USkeletalMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName /*= "WeaponMesh"*/)
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
	}
}


/*
 * �֐����@�@�@�@�FAttachToHand()
 * �������e�@�@�@�F�L�����N�^�[�ɂ�������
 * �����P�@�@�@�@�FACharacter* _owner�E�E�E�e�ɂȂ�L�����N�^�[
 * �����Q�@�@�@�@�FFName _socketName �E�E�E�\�P�b�g�̖��O
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::AttachToHand(ACharacter* _owner, FName _socketName)
{
	// ������Ă����Ԃɂ���
	IsHeld = true;

	// �L�����N�^�[�ɃA�^�b�`
	if (_owner)
	{
		AttachToActor(_owner, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative,false), _socketName);
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
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative,false));

	// ������΂�
	BlowsAway();
}


/*
 * �֐����@�@�@�@�FDetachToHand()
 * �����P�@�@�@�@�FACharacter* _owner�E�E�E�e�ɂȂ�L�����N�^�[
 * �����Q�@�@�@�@�FFName _ownerTag �E�E�E�E�I�[�i�[�ɂ��Ă���^�O
 * �����R�@�@�@�@�FFName _socketName �E�E�E�\�P�b�g�̖��O
 * �������e�@�@�@�F�I�[�i�[�̏�񏉊���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::InitializeOwnerData(ACharacter* _owner, FName _ownerTag, FName _socketName)
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

}
