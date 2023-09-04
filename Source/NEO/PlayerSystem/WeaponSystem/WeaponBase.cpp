// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "GameFramework/Character.h"
#include "NEO/PlayerSystem/ActionAssistComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NEO/PlayerSystem/ActionAssistComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
	: IsHeld(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// �^�O�ݒ�
	Tags.Add("Weapon");

	// �A�^�b�N�A�V�X�g�R���|�[�l���g�쐬
	ActionAssistComp = CreateDefaultSubobject<UActionAssistComponent>(TEXT("AttackAssist"));
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
void AWeaponBase::AttachToHand(ACharacter* _owner, FName _socketName)
{
	// ������Ă����Ԃɂ���
	IsHeld = true;

	
	// �L�����N�^�[�ɃA�^�b�`
	if (_owner)
	{
		AttachToComponent(_owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false), _socketName);
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

	// ������΂�
	BlowsAway();
}


/*
 * �֐����@�@�@�@�FSetupOwnerData()
 * �����P�@�@�@�@�FACharacter* _owner�E�E�E�e�ɂȂ�L�����N�^�[
 * �����Q�@�@�@�@�FFName _ownerTag �E�E�E�E�I�[�i�[�ɂ��Ă���^�O
 * �����R�@�@�@�@�FFName _socketName �E�E�E�\�P�b�g�̖��O
 * �������e�@�@�@�F�I�[�i�[�̏�񏉊���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AWeaponBase::SetupOwnerData(AActor* _owner, FName _ownerTag, FName _socketName)
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
	// ��]
	SetActorRotation(FRotator(0.f, 0.f, -90.f));
}
