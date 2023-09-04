// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "NEO/GameSystem/InputCharacter.h"
#include <unordered_map>
#include <type_traits>
#include "ActionAssistComponent.h"
#include <Runtime/Engine/Classes/Components/CapsuleComponent.h>

#include "PlayerBase.generated.h"

// �O���錾
class UBoxComponent;
class USphereComponent;
class UCapsuleComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class USplineComponent;


//-----------------inputAction------------------------------------------------------------
USTRUCT(BlueprintType)
struct FMainAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UInputMappingContext* DefaultMappingContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UInputAction* MoveAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UInputAction* RunAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UInputAction* JumpAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UInputAction* ComboAction1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UInputAction* ComboAction2 = nullptr;
};
//----------------------------------------------------------------------------------------

//-----------------�v���C���[�̃X�e�[�^�X-------------------------------------------------
USTRUCT(BlueprintType)
struct FPlayerStatus
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float HP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float MaxHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int WeaponDropLimit = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int DefaultWeaponDropLimit = WeaponDropLimit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int RemainingLife = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float DamageAmount = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float ComboDamageFactor = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float JumpHeight = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float WalkSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float RunSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float NowMoveSpeed = 600.f;
};
//----------------------------------------------------------------------------------------

//-----------------�A�j���[�V�����ۊǗp---------------------------------------------------
USTRUCT(BlueprintType)
struct FPlayerAnimation
{
	GENERATED_BODY()

	// �R���{
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		TArray<UAnimMontage*> ComboAttack = { nullptr,nullptr };

	// �󒆂ɂ���Ƃ��̍U��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* AirAttack = nullptr;

	// ��_���[�W
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* TakeDamage = nullptr;

	// ��_���[�W
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* KnockBack = nullptr;

	// ���S��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* Death = nullptr;
};
//----------------------------------------------------------------------------------------

UCLASS()
class NEO_API APlayerBase : public AInputCharacter
{
	GENERATED_BODY()

public:

	// �R���X�g���N�^
	APlayerBase();

protected:

	//----------------���͂ŌĂяo�����֐�-----------------------------
	// �ړ�
	virtual void Move(const FInputActionValue& _value);

	// �_�b�V���؂�ւ�
	void Run();

	// �W�����v
	virtual void JumpStart();
	virtual void Jump();

	// �U��
	virtual void Attack(int _attackNum = 0);

	// ��ڂ̃R���{
	virtual void Combo1();

	// ��ڂ̃R���{
	virtual void Combo2();
	//-------------------------------------------------------------------


public:

	//-----------------���N���X�ŌĂяo����--------------------------------------------------------------------------------------------
	// �R���{�p��
	void ContinuationCombo();

	// �R���{���Z�b�g
	void ResetCombo();

	int GetComboIndex()const { return ComboIndex; }

	// �_���[�W��^���鏈��
	virtual void SetCollision() { return; }

	// �A�N�V�����A�V�X�g�R���|�[�l���g���擾
	class UActionAssistComponent* GetActionAssistComponent()const { return ActionAssistComp; }

	// �q�b�g�X�g�b�v
	void HitStop() { ActionAssistComp->HitStop(HitStopTime); }

	// �J�����V�F�C�N
	void CameraShake() { ActionAssistComp->CameraShake(ShakePattern); }

	// �G�t�F�N�g�X�|�[��
	void SpawnEffect() { ActionAssistComp->SpawnEffect(HitEffect,GetActorLocation()); }
	
	// ����E�s��؂�ւ��鏈��
	void SetControl(bool _isControl) { IsControl = _isControl; }

	// ���S���̃A�j���[�V�����̍Đ���x������
	void SlowDownDeathAnimationRate();

	// �_���[�W�ʂ�Ԃ��֐�
	UFUNCTION(BlueprintCallable, Category = "Action")
		float GetDamageAmount()const { return PlayerStatus.DamageAmount * (((float)ComboIndex + 1.f) * PlayerStatus.ComboDamageFactor); }

	// ���݂�HP��Ԃ�
	UFUNCTION(BlueprintCallable, Category = "GetStatus")
		float GetHP()const { return PlayerStatus.HP; }

	// ���݂̈ړ��ʂ�Ԃ�
	float GetDistanceAdvanced() { return DistanceAdvanced * deltaTime;}

	// �_���[�W���󂯂鏈��
	UFUNCTION(BlueprintCallable, Category = "Action")
		void TakedDamage(float _damage, bool _isLastAttack = false);

	// �v���C���[���n�ʂɂ��邩
	bool IsPlayerGrounded()const;

	// �ڐG�J�n���ɍs������
	UFUNCTION()
		virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//---------------------------------------------------------------------------------------------------------------------------------------------------


private:

	// �L�����N�^�[�̉�]
	void RotateCharacter(float _nowInput_X);

	// �L�����N�^�[�̈ړ��ʎ擾
	void AmountOfMovement(FVector _nowPos);

	// ���S�����Ăяo��
	void CallGameModeFunc_DestroyPlayer();

	// �X�v���C������
	AActor* GetSplineActor(const FName _tag);

	// �A�j���[�V�����Đ�
	void PlayAnimation(UAnimMontage* _toPlayAnimMontage, FName _startSectionName = "None", float _playRate = 1.f);

protected:

	// �Q�[���J�n���ɌĂяo����鏈��
	virtual void BeginPlay() override;

	// ���t���[���Ăяo����鏈��
	virtual void Tick(float DeltaTime) override;

	// ���͂̃Z�b�g�A�b�v
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//------------------�v���C���[�̃Z�b�g�A�b�v---------------------------------------------------------------------------------------------------------
	// �v���C���[�̃f�[�^��������
	virtual void SetupPlayerData();

	// �v���C���[�̃X�e�[�^�X������
	void SetupPlayerStatus(float _hp = 100.f, int _remainingLife = 3.f, float _damageAmount = 10.f,
							float _jumpHeight = 150.f, float _comboDamageFactor = 1.f, float _walkSpeed = 10.f, float _runSpeed = 30.f);

	// �{�^���̐ݒ�
	void SetupMainActionMapping();

	// �����ɂ���ăX�^�e�B�b�N���b�V�����X�P���^�����b�V���̃Z�b�g�A�b�v
	void SetupWeaponMesh(UStaticMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName = "WeaponStaticMesh");
	void SetupWeaponMesh(USkeletalMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName = "WeaponSkeletalMesh");

	// �w�肵���p�X�̃A�j���[�V�����A�Z�b�g��Ԃ�
	UAnimMontage* GetAnimationAsset(TCHAR* _animAssetPath);

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
			CollisionComp->SetupAttachment(GetMesh(), "hand_rSocket");
		}
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------


	//-----------------�v���C���[�Ǘ��p�ϐ�------------------------------------------------------------------
private:

	// �v���C��[�̃A�N�V�����Ǘ��p
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FMainAction MainActionMapping;

protected:

	// �v���C���[�̃X�e�[�^�X�Ǘ��p
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FPlayerStatus PlayerStatus;

	// �v���C���[�̃A�j���[�V�����Ǘ��p
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FPlayerAnimation PlayerAnimation;

	// �v���C���[�̕���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AWeaponBase> WeaponClass;

	//-------------------------------------------------------------------------------------------------------------


	//-----------------�R���|�[�l���g�ϐ�--------------------------------------------------------------------------
	// �U���̃A�V�X�g�p
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action Assist", meta = (AllowPrivateAccess = "true"))
		class UActionAssistComponent* ActionAssistComp;

	// ��_���[�W���̃G�t�F�N�g
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
		class UNiagaraSystem* HitEffect;

	// �J�����̗h���
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UCameraShakeBase> ShakePattern;

	// �L�����N�^�[�̓���
	UCharacterMovementComponent* CharacterMovementComp;
	//-------------------------------------------------------------------------------------------------------------

	// ���b�ԃq�b�g�X�g�b�v���N������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Action Assist")
		float HitStopTime = 0.2f;

	// ���S�A�j���[�V�����œ|��Ă���̍Đ��X�s�[�h(1�Œʏ�X�s�[�h)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"),Category = "GameOver")
		float DeadAnimRate = 0.01f;							

	// ����ł���Q�[���I�[�o�[�܂ł̎���(�b)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float DeadToGameOverTime = 3.f;					

	// �ړ���������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float DistanceAdvanced;

	// ����̃N���X
	class AWeaponBase* Weapon;

private:

	// ���͉\���ǂ���
	bool IsControl;					

	// �_�b�V�������ǂ���
	bool IsRunning;		

	// �W�����v�����ǂ���
	bool IsJumping;

	// ����������Ă��邩�ǂ���
	bool IsHoldWeapon;

	// ����ł��邩�ǂ���
	bool IsDeath;

	// �t���[���J�E���g�p
	float frames;	

	// �W�����v�̌v�Z
	const float radPerFrame = 3.14f / 30.f;

	// �W�����v�O�̍���
	float JumpBeforePos_Z;	

	// �O�̈ʒu
	FVector BeforePos;

	// �U�������ǂ���
	bool IsAttacking;						

	// �R���{�p���ł��邩
	bool CanCombo;							

	// ���i�ڂ̃R���{��
	int ComboIndex;							

	// �R���{�̒i��(First,Second,Third�E�E�E)
	TArray<FName> ComboStartSectionNames;	

	// �n���h��
	FTimerHandle TimerHandle_DeathToGameOver;		

	// �v���C���[���ʂ�X�v���C��
	class APlayerSpline* SplineActor;		

	// �Q�[�����[�h�ۑ�
	class ATGS_GameMode* pGameMode;

	// �f���^�^�C���ۑ��p
	float deltaTime;

//////////////////////////////////////////////////////////////////////////
///UI
public:
	//�v���C���[��HP�́���Ԃ�
	UFUNCTION(BlueprintCallable, Category = "UI")
		float GetPlayerHPPercent()const { return PlayerStatus.HP / PlayerStatus.MaxHP; }

	class UTGS_GameInstance* GetGameInstance();

};