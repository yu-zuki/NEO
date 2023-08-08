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
//----------------------------------------------------------------------------------------

//-----------------�v���C���[�̃X�e�[�^�X-------------------------------------------------
USTRUCT(BlueprintType)
struct FPlayerStatus
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float HP;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int RemainingLife;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float ComboDamageFactor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float JumpHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float NowMoveSpeed;
};
//----------------------------------------------------------------------------------------

//-----------------�A�j���[�V�����ۊǗp-----------------------------------------------------
USTRUCT(BlueprintType)
struct FPlayerAnimation
{
	GENERATED_BODY()

		// �R���{
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation)
		TArray<UAnimMontage*> Combo;

	// ��_���[�W
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation)
		UAnimMontage* TakeDamage;

	// ���S��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation)
		UAnimMontage* Death;
};
//----------------------------------------------------------------------------------------


UCLASS()
class NEO_API APlayerBase : public AInputCharacter
{
	GENERATED_BODY()

	// �v���C���[�̃X�e�[�g
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

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FPlayerAnimation PlayerAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UCameraShakeBase> ShakePattern;

public:
	// Sets default values for this character's properties
	APlayerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	//----------------���͂ŌĂяo�����֐�-----------------------------
	// �ړ�
	void Move(const FInputActionValue& _value);

	// �_�b�V���؂�ւ�
	void Run();

	// �W�����v
	void JumpStart();
	void Jump();

	// �v���C���[���n�ʂɂ��邩
	bool IsPlayerGrounded()const;

	// �U��
	virtual void Attack(int _attackNum = 0);

	// ��ڂ̃R���{
	virtual void Combo1();

	// ��ڂ̃R���{
	virtual void Combo2();
	//-------------------------------------------------------------------

protected:

	//-----------------AnimBP�ŌĂяo��(�U�����_���[�W����)--------------------------------------------------------------------------------------------
	// �R���{�p��
	UFUNCTION(BlueprintCallable, Category = "Action")
		void ContinuationCombo();

	// �R���{���Z�b�g
	UFUNCTION(BlueprintCallable, Category = "Action")
		void ResetCombo();

	UFUNCTION(BlueprintCallable, Category = "Action")
		int GetComboIndex()const { return ComboIndex; }

	// �_���[�W��^���鏈��
	UFUNCTION(BlueprintCallable, Category = "Action")
		virtual void SetCollision();


	UFUNCTION(BlueprintCallable, Category = "Action")
		void SetControl(bool _isControl) { IsControl = _isControl; }

	// ���S���̃A�j���[�V�����̍Đ���x������
	UFUNCTION(BlueprintCallable, Category = "Action")
		void SlowDownDeathAnimationRate();

	// �_���[�W�ʂ�Ԃ��֐�
	UFUNCTION(BlueprintCallable, Category = "GetStatus")
		float GetDamageAmount()const { return PlayerStatus.DamageAmount * (((float)ComboIndex + 1.f) * PlayerStatus.ComboDamageFactor); }

	// ���݂�HP��Ԃ�
	UFUNCTION(BlueprintCallable, Category = "GetStatus")
		float GetHP()const { return PlayerStatus.HP; }
	//---------------------------------------------------------------------------------------------------------------------------------------------------


	// �A�j���[�V�����Đ�
	void PlayAnimation(UAnimMontage* _toPlayAnimMontage, FName _startSectionName = "None", float _playRate = 1.f);

private:

	// �L�����N�^�[�̉�]
	void RotateCharacter(float _nowInput_X);

	// ���S�����Ăяo��
	void CallGameModeFunc_DestroyPlayer();

	// �X�v���C������
	AActor* GetSplineActor(const FName _tag);


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �_���[�W���󂯂鏈��
	UFUNCTION(BlueprintCallable, Category = "Action")
		void TakedDamage(float _damage);

	//------------------�v���C���[�̃Z�b�g�A�b�v---------------------------------------------------------------------------------------------------------
	// �v���C���[�̃f�[�^��������
	virtual void SetupPlayerData();

	// �v���C���[�̃X�e�[�^�X������
	void SetupPlayerStatus(float _hp = 100.f, int _remainingLife = 3.f, float _damageAmount = 10.f,
							float _jumpHeight = 150.f, float _comboDamageFactor = 1.f, float _walkSpeed = 10.f, float _runSpeed = 30.f);

	// �{�^���̐ݒ�
	void SetupMainActionMapping();

	// �����ɂ���ăX�^�e�B�b�N���b�V�����X�P���^�����b�V���̃Z�b�g�A�b�v
	void SetupWeaponMesh(UStaticMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName = "WeaponMesh");
	void SetupWeaponMesh(USkeletalMeshComponent*& MeshComp, TCHAR* WeaponAssetPath, FName PublicName = "WeaponMesh");

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


	//-----------------�R���|�[�l���g�ϐ�--------------------------------------------------------------------------
	// ����̃��b�V��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* WeaponMesh;

	// �U���̃A�V�X�g�p�R���|�[�l���g
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AttackAssist, meta = (AllowPrivateAccess = "true"))
		class UActionAssistComponent* ActionAssistComp;

	// �L�����N�^�[�̓���
	UCharacterMovementComponent* CharacterMovementComp;
	//-------------------------------------------------------------------------------------------------------------

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Attack")
		float HitStopTime = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "GameOver")
		bool CurveMode;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"),Category = "GameOver")
		float DeadAnimRate;							// ���S�A�j���[�V�����œ|��Ă���̍Đ��X�s�[�h(1�Œʏ�X�s�[�h)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float DeadToGameOverTime;					// ����ł���Q�[���I�[�o�[�܂ł̎���(�b)


	float deltaTime;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float DistanceAdvanced;

private:

	FTimerHandle TimerHandle_DeathToGameOver;		// �n���h��

	class APlayerSpline* SplineActor;				// �v���C���[���ʂ�X�v���C��

//////////////////////////////////////////////////////////////////////////
///UI
public:
	//�v���C���[��HP�́���Ԃ�
	UFUNCTION(BlueprintCallable, Category = "UI")
		float GetPlayerHPPercent()const { return PlayerStatus.HP / PlayerStatus.MaxHP; }

	class UTGS_GameInstance* GetGameInstance();

};