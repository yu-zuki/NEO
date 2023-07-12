// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GameSystem\InputCharacter.h"
#include "PlayerBase.generated.h"

// �O���錾
class USphereComponent;
class UCapsuleComponent;
class UBoxComponent;

// inputAction
USTRUCT(BlueprintType)
struct FMainAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UInputAction* RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UInputAction* ComboAction1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UInputAction* ComboAction2;
};

// inputAction
USTRUCT(BlueprintType)
struct FPlayerStatus
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float HP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float ComboDamageFactor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float JumpHeight;
};

UCLASS()
class NEO_API APlayerBase : public AInputCharacter
{
	GENERATED_BODY()

	// �v���C���[
	enum Player_State
	{
		State_Idle = 0,
		State_Jump,
		State_Death
	};


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FMainAction MainActionMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FPlayerStatus PlayerStatus;

public:
	// Sets default values for this character's properties
	APlayerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	// �_�b�V���؂�ւ�
	void Run();

	// �W�����v
	void JumpStart();
	void Jump();

	// �v���C���[���n�ʂɂ��邩
	bool IsPlayerGrounded()const;

	// �U��
	void Attack(int AttackNum = 0);

	// ��ڂ̃R���{
	void Combo1();

	// ��ڂ̃R���{
	void Combo2();

protected:

	// �L�����N�^�[�̉�]
	void RotateCharacter(float nowInput_Y);

	// �R���{�p��
	UFUNCTION(BlueprintCallable,Category = "ComboAction")
		void ContinuationCombo();

	// �R���{���Z�b�g
	UFUNCTION(BlueprintCallable, Category = "ComboAction")
		void ResetCombo();

	// �_���[�W��^���鏈��
	UFUNCTION(BlueprintCallable, Category = "ComboAction")
		virtual void SetoCollision();

	// �_���[�W�ʂ�Ԃ��֐�
	UFUNCTION(BlueprintCallable,Category = "GetStatus")
		float GetDamageAmount()const { return PlayerStatus.DamageAmount * (((float)ComboIndex + 1.f) * PlayerStatus.ComboDamageFactor); }

	// �_���[�W���󂯂鏈��
	UFUNCTION(BlueprintCallable, Category = "GetStatus")
		void TakedDamage(float _damage);

	UFUNCTION(BlueprintCallable, Category = "GetStatus")
		float GetHP()const { return PlayerStatus.HP; }

	// �A�j���[�V�����Đ�
	void PlayAnimation(UAnimMontage* _toPlayAnimMontage, FName _startSectionName = "None", float _playRate = 1.f);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �v���C���[�̃f�[�^��������
	virtual void SetupPlayerData();

	// �v���C���[�̃X�e�[�^�X�p�����[�^������
	void SetupPlayerStatus(float _hp = 100.f, float _damageAmount = 10.f, float _jumpHeight = 150.f, float _comboDamageFactor = 1.f);

	// �{�^���̐ݒ�
	void SetupMainActionMapping();

	// �A�j���[�V�����̐ݒ�
	void SetupAnimationAsset(TCHAR* AnimAssetPath[2]);

	// ����̃��b�V���̐ݒ�
	void SetupWeaponMesh(TCHAR* WeaponAssetPath,FName PublicName = "WeaponMesh");


	// �e���v���[�g
	// �R���W�����ݒ�
	template<class T>
	void SetupCollisionComponent(T CollisionComp, FName PublicName = "CollisionComp");

	// ����̃��b�V��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* WeaponMesh;

	// �A�j���[�V�����ۊǗp
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation)
		TArray<UAnimMontage*> ComboAnimMontages;

	// �L�����N�^�[�̓���
	UCharacterMovementComponent* CharacterMovementComp;

protected:

	bool IsControl;					// ���͉\���ǂ���

	bool IsRunning;					// �_�b�V�����̃t���O

	float frames;					// �t���[��		

	const float radPerFrame = 3.14f / 30.f;

	float JumpBeforePos_Z;					// �W�����v�O�̍���

	Player_State PlayerState;				// �v���C���[�̃X�e�[�g

	bool IsAttacking;						// �U�����̃t���O

	bool CanCombo;							// �R���{�p���ł��邩

	int ComboIndex;							// ���i�ڂ̃R���{��

	TArray<FName> ComboStartSectionNames;	// �R���{�̒i��(First,Second,Third�E�E�E)
};
