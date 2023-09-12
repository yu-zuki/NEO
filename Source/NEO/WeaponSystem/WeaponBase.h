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
		class ACharacter* pOwner = nullptr;

	// �I�[�i�[�̃^�O
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FName OwnerTag = "None";

	// �A�^�b�`����\�P�b�g�̖��O
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FName SocketName = "None";
};
//----------------------------------------------------------------------------------------

//-----------------�����҂̎��-------------------------------------------------------------
UENUM(BlueprintType)
enum class EOwnerType :uint8
{
	OwnerType_Player   UMETA(DisplayName = "PlayerType"),
	OwnerType_Enemy    UMETA(DisplayName = "EnemyType"),
	OwnerType_Boss     UMETA(DisplayName = "BossType"),
};
//----------------------------------------------------------------------------------------

//-----------------����̎��-------------------------------------------------------------
UENUM(BlueprintType)
enum class EWeaponType :uint8
{
	WeaponType_Sword   UMETA(DisplayName = "Sword"),
	WeaponType_Lance   UMETA(DisplayName = "Lance"),
	WeaponType_Gun     UMETA(DisplayName = "Gun"),
	WeaponType_None	   UMETA(DisplayName = "None")
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


	// StaticMesh�̃Z�b�g�A�b�v
	void SetupWeaponMesh(UStaticMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName = "WeaponMesh");

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

protected:

	// �I�[�i�[�̃f�[�^������
	void SetupOwnerData(ACharacter* _owner,FName _ownerTag,FName _socketName);

	// �O�ꂽ���������
	void BlowsAway();

	// �v���C���[�̍U���L�q�p
	virtual void PlayerAttack() { return; }

	// �G�̍U���L�q�p
	virtual void EnemyAttack() { return; }

	// �{�X�̍U���L�q�p
	virtual void BossAttack() { return; }

public:

	// �v���C���[�̎�ɕt����
	void AttachToHand(ACharacter* _owner, FName _socketName, EOwnerType _ownerType);

	// �v���C���[�̎肩��O���
	void DetachToHand();

	// �_���[�W��^���鏈��(�I�[�o�[���C�h�p)
	virtual void SetCollision() { return; }

	// ������Ă����Ԃ�
	UFUNCTION(BlueprintCallable, Category = "State")
		bool GetIsHeld()const { return IsHeld; }

	// ���ł����Ԃ�
	UFUNCTION(BlueprintCallable, Category = "State")
		bool GetIsFalling()const { return IsFalling; }

	// �I�[�i�[�̎�ޔ��ʗp
	UFUNCTION(BlueprintCallable, Category = "State")
		EOwnerType GetOwnerType()const { return OwnerType; }

	// ����̎�ޔ��ʗp
	EWeaponType GetWeaponType()const { return WeaponType; }

	// �I�[�i�[�̏��
	FOwnerInfo OwnerInfo;

	// ����̃��b�V��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponMesh")
		class UStaticMeshComponent* WeaponStaticMesh;

	// ����̃��b�V��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponMesh")
		class UCapsuleComponent* WeaponCollision;

	// �I�[�i�[�𔻕ʂ���Enum
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OwnerType")
		EOwnerType OwnerType;

	// ����𔻕ʂ���Enum
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponType")
		EWeaponType WeaponType;

protected:

		// �A�N�V�����A�V�X�g�R���|�[�l���g
		class UActionAssistComponent* ActionAssistComp;


private:

	// �v���C���[�Ɏ�����Ă��邩�̃t���O
	bool IsHeld;

	// ���ł��邩�ǂ���
	bool IsFalling;

	// �t���[���J�E���g�p
	float frames;

	// �W�����v�̌v�Z
	const float radPerFrame = 3.14f / 30.f;

	// ��ԑO�̈ʒu
	FVector FlyBeforePos;

	// ��_���[�W���̃G�t�F�N�g
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
		class UNiagaraSystem* AuraEffect;
};
