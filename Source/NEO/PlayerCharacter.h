// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UInputAction;
class UInputMappingContext;
class USkeletalMeshComponent;
class UCharacterMovementComponent;

UCLASS()
class NEO_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	// ����
	enum Attribute_State
	{
		State_Fire = 0,
		State_Ice,
		State_Wind
	};

private:

	// �R���{
	enum Attack_State
	{
		State_Combo1 = 0,
		State_Combo2,
		State_Ult
	};

	// �v���C���[
	enum Player_State
	{
		State_Idle = 0,
		State_Jump
	};

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* RunAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* SwitchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* ComboAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* ComboAction2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* UltAction;


public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	void Attack();

	// ��ڂ̃R���{
	void Combo1();

	// 2�ڂ̃R���{
	void Combo2();

	// �R���{���Z�b�g
	void ResetCombo();

	// �K�E�Z
	void UltimateAttack();

	// �_���[�W���󂯂鏈��
	void TakedDamage();

	// �����؂�ւ�
	void SwitchAttribute();

	// �A�j���[�V�����Đ�
	void PlayAnimation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �{�^���̐ݒ�
	void SetupDefoultMappingContext();

	// �A�j���[�V�����̐ݒ�
	void SetupAnimationAsset();

	UFUNCTION(BlueprintCallable)
		int GetAttribute()const { return AttrState; }


	// ���̃��b�V��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* SwordMesh;

	// �A�j���[�V�����ۊǗp
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation)
		TArray<UAnimMontage*> ComboAnimMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation)
		UAnimMontage* UltAnimMontage;

	// �L�����N�^�[�̓���
	UCharacterMovementComponent* CharacterMovementComp;


private:

	bool IsControl;					// ���͉\���ǂ���

	bool IsRunning;					// �_�b�V�����̃t���O

	float frames;					// �t���[��		

	UPROPERTY(EditAnywhere)
		float height = 150;			// �W�����v�̍���

	const float radPerFrame = 3.14f / 30.f;

	float JumpBeforePos_Z;			// �W�����v�O�̍���

	Player_State PlayerState;		// �v���C���[�̃X�e�[�g

	Attribute_State AttrState;		// �����̃X�e�[�g

	bool IsAttacking;				// �U�����̃t���O

	Attack_State AttackState;		// �U���̃X�e�[�g

	bool CanCombo;					// �R���{�p���ł��邩

	int ComboIndex;					// ���i�ڂ̃R���{��

	TArray<FName> ComboCntNames;	// ���i�ڂ̃R���{�̖��O(First,Second,Third�E�E�E)

	UPROPERTY(EditAnywhere, Category = Damage)
	float DamageAmount;				// �_���[�W��
};
