// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"


class ACharacter;

//-----------------OwnerInformation-------------------------------------------------------
USTRUCT(BlueprintType)
struct FOwnerInfo
{
	GENERATED_BODY()

public:

	// �I�[�i�[�ɂȂ�L�����N�^�[�̏��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		ACharacter* pOwner;

	// �I�[�i�[�̃^�O
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FName OwnerTag;

	// �A�^�b�`����\�P�b�g�̖��O
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FName SocketName;
};
//----------------------------------------------------------------------------------------

UCLASS()
class NEO_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// �ڐG�J�n���ɍs������
	UFUNCTION()
		virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// �����ɂ���ăX�^�e�B�b�N���b�V�����X�P���^�����b�V���̃Z�b�g�A�b�v
	void SetupWeaponMesh(UStaticMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName = "WeaponMesh");
	void SetupWeaponMesh(USkeletalMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName = "WeaponMesh");


// ---------�R���W�����R���|�[�l���g�쐬�e���v���[�g
/*
 * �֐����@�@�@�@�FSetupCollisionComponent()
 * �������e�@�@�@�F����̃R���W�����Z�b�g�A�b�v
 * �����P�@�@�@�@�FT* CollisionComp�E�E�E�E�E�E�E�E�E�R���W�����R���|�[�l���g(�uT�v�� UBoxComponent,USphereComponent,UCapsuleComponent �̂����ꂩ)
 * �����Q�@�@�@�@�FFName PublicName�E�E�E�E�E�E�E�E�E�G�f�B�^�ł̌��J��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
	template<class T>
	void SetupCollisionComponent(T*& CollisionComp, FName PublicName = "CollisionComp")
	{
		static_assert(std::is_same<T, UBoxComponent>::value || std::is_same<T, USphereComponent>::value || std::is_same<T, UCapsuleComponent>::value,
			"�uT�v�� UBoxComponent,USphereComponent,UCapsuleComponent �̂����ꂩ ");

		// �Ή�����I�u�W�F�N�g����
		CollisionComp = CreateDefaultSubobject<T>(PublicName);

		if (CollisionComp)
		{
			// ����̃��b�V���ɒǏ]
			CollisionComp->SetupAttachment(WeaponStaticMesh, "hand_rSocket");
		}
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------

public:	

	// �v���C���[�̎�ɕt����
	void AttachToHand(ACharacter* ParentCharacter, FName SocketName);

	// �v���C���[�̎肩��O���
	void DetachToHand();


protected:

	// �I�[�i�[�̃f�[�^������
	void InitializeOwnerData(ACharacter* _owner,FName _ownerTag,FName _socketName);

	// �O�ꂽ���������
	void BlowsAway();

	// �_���[�W��^���鏈��(�I�[�o�[���C�h�p)
	virtual void SetCollision() { return; }

private:

	FOwnerInfo OwnerInfo;

	// �v���C���[�Ɏ�����Ă��邩�̃t���O
	bool IsHeld;
};
