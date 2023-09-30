// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"


class ACharacter;
class UNiagaraSystem;

//-----------------�����҂̎��-----------------------------------
UENUM(BlueprintType)
enum class EOwnerType :uint8
{
	OwnerType_Player   UMETA(DisplayName = "PlayerType"),
	OwnerType_Enemy    UMETA(DisplayName = "EnemyType"),
	OwnerType_Boss     UMETA(DisplayName = "BossType")
};
//----------------------------------------------------------------

//-----------------����̎��-------------------------------------
UENUM(BlueprintType)
enum class EWeaponType :uint8
{
	WeaponType_Sword   UMETA(DisplayName = "Sword"),
	WeaponType_Lance   UMETA(DisplayName = "Lance"),
	WeaponType_Gun     UMETA(DisplayName = "Gun"),
	WeaponType_None	   UMETA(DisplayName = "None")
};
//----------------------------------------------------------------


UCLASS()
class NEO_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// �R���X�g���N�^
	AWeaponBase();

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

	// �v���C���[���߂��ɂ��邩
	UFUNCTION(BlueprintCallable, Category = "State")
		bool GetIsPickUpDistanceEnteredPlayer()const { return IsPickUpDistanceEnteredPlayer; }

	// �I�[�i�[�̎�ޔ��ʗp
	UFUNCTION(BlueprintCallable, Category = "State")
		EOwnerType GetOwnerType()const { return OwnerType; }

	// ����̎�ޔ��ʗp
	UFUNCTION(BlueprintCallable, Category = "State")
		EWeaponType GetWeaponType()const { return WeaponType; }


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


	// �v���C���[�̍U���L�q�p
	virtual void PlayerAttack() { return; }

	// �G�̍U���L�q�p
	virtual void EnemyAttack() { return; }

	// �{�X�̍U���L�q�p
	virtual void BossAttack() { return; }

	// �U���������������ǂ���
	bool GetHitResults(FVector _start,FVector _end, TArray<FHitResult>& _outHitResults);

	// �j��\�I�u�W�F�N�g�ɍU���������̏���
	void AttackObject(AActor* _object, float _damageAmount, class USoundBase* _hitSoundObj);

	// �G�ɍU���������̏���
	virtual void AttackEnemy(AActor* _enemy, float _damageAmount, class USoundBase* _hitSoundObj);

	// �G�ɍU���������̃q�b�g�X�g�b�v�̎��Ԃ�ݒ�
	virtual float SetHitStopTime(int _comboNum) { return 0.f; }

	// �G�̃m�b�N�o�b�N����
	void EnemyKnockBack(int _comboNum, class AEnamyBase* _enemy);


private:

	// �v���C���[�Ƃ̋������v�Z���鏈��
	void DistanceCalculationToPlayer();

	// �I�[�i�[�����Ȃ��Ȃ����Ƃ����킪������
	void BlowsAway();


protected:

		// ����̃��b�V��
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponMesh")
			class UStaticMeshComponent* WeaponStaticMesh;

		// ����̃R���W����
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponCollision")
			class UCapsuleComponent* WeaponCollision;

		// �A�N�V�����⏕�̖���������R���|�[�l���g
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionAssist")
			class UActionAssistComponent* ActionAssistComp;

		// �����Ă��镐��ɂ�����G�t�F�N�g
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
			UNiagaraSystem* AuraEffect;

		// ����𗎂Ƃ������̉�
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
			class USoundBase* DropWeaponSoundObj;

		// ����𔻕ʂ���Enum
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
			FVector DropLocation;

		// ����𔻕ʂ���Enum
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
			FRotator DropAngle;

		// �v���C���[�Ƃ̋������߂����̌v�Z���s���Ԋu
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calculate")
			float CalculateInterval_nearPlayer;

		// �v���C���[�Ƃ̋������������̌v�Z���s���Ԋu
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calculate")
			float CalculateInterval_farPlayer;

		// �����v�Z���Ȃ�����
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calculate")
			float DistanceToOmitCalc;

		// �W�����v�̍���
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calculate")
			float JumpHeight;

		// �����҂̏��
		ACharacter* pOwner;

		// �I�[�i�[�𔻕ʂ���Enum
		EOwnerType OwnerType;

		// ����𔻕ʂ���Enum
		EWeaponType WeaponType;

private:

	// �v���C���[�Ɏ�����Ă��邩
	bool IsHeld;

	// ���ł��邩�ǂ���
	bool IsFalling;

	// �v���C���[����苗�����ɓ�������
	bool IsPickUpDistanceEnteredPlayer;

	// �t���[���J�E���g�p
	float Frames;

	// �v���C���[�Ƃ̋���
	float DistanceToPlayer;

	// �W�����v�̌v�Z
	const float RadPerFrame = 3.14f / 30.f;

	// ��ԑO�̈ʒu
	FVector FlyBeforePos;

	// �v���C���[�R���g���[���[�̏��
	class ANEOPlayerController* PlayerController;

	// �n���h��
	FTimerHandle TimerHandle;
};
