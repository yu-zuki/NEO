// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPPOdaEnum.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animation/AnimMontage.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"		//RandRange���g�����߂ɕK�v�ȃw�b�_�[
#include "Components/BoxComponent.h"
#include "Engine/EngineTypes.h"
#include "Components/StaticMeshComponent.h"
#include "Templates/SubclassOf.h"
#include "PlayerCharacter.h"

#include "OdaBase.generated.h"


UCLASS()
class NEO_API AOdaBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOdaBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//�V�[���R���|�[�l���g
	UPROPERTY()
		USceneComponent* Parent;

	//�{�X�L�����N�^�[
	UPROPERTY()
	AOdaBase* OdaNobunaga;

	//���̃R���W����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BoxComp", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComp;

	//�L�����N�^�[���[�u�����g
	UPROPERTY()
		UCharacterMovementComponent* NobunagaMovement;

	//�_���[�W����
	UFUNCTION()
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);

	//�_���[�W�^�C�v�N���X
	UPROPERTY()
		TSubclassOf < class UDamageType > DamageTypeClass;

	//���Ԃ̎擾
	int FlameCounter;

	//�O�������Ƃ�
	UPROPERTY()
	FVector ForwardDirection;

	//�񋓌^
	ECPPOdaEnum OdaMoveEnum;

	//�ҋ@�֐�
	UPROPERTY()
	bool SwitchStayMove;

	//int�^�̃J�E���^�[
	int WaitTime;


	//�{�X�̃X�e�[�g�ł̏���
	void OdaStay1(int Timer);

	void OdaMove1(int Timer);

	void OdaMove2(int Timer);

	void OdaBack1(int Timer);

	void OdaAttack1(int Timer);

	void OdaAttack2(int Timer);


	//�ʏ�̈ړ����x
	UPROPERTY(EditAnywhere)
		float OdaSpeed;

	//�}�ȑ��x
	UPROPERTY(EditAnywhere)
		float FastOdaSpeed;
	
	//�s���ύX���Ԑݒ�
	UPROPERTY(EditAnywhere)
		int ChangeFlontTimer;

	//���[�V��������񂾂��������߂̕ϐ�
	bool isMotionPlaying;

	//�Ռ��g�̏o���^�C�~���O�̒���
	UPROPERTY()
		bool isShockWaveSpawnTiming;

	//�Ռ��g�̏o���^�C�~���O�̒������邽�߂̊֐�
	UFUNCTION()
		void ShockWaveSpawnFlagChange();

	//�Ռ��g���o��������ׂ̕ϐ�
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor>ShockWaveSpawn;

	//�U���̃t���O
	UPROPERTY(BlueprintReadOnly)
		bool IsAttackNow;

	//�U���̃t���O��؂�ւ���ׂ̊֐�
	UFUNCTION()
		void AttackFlagChange();

	//�̗�
	UPROPERTY()
	float Health;

	//HP���A���Ō���Ȃ��悤�Ƀ��b�N
	UPROPERTY()
		bool isHPLock;

	//HP���A���Ō���Ȃ��悤�Ƀ��b�N���邽�߂̊֐�
	UFUNCTION()
		void HPLock();

	//�v���C���[�̕����Ɍ����鏈��
	UFUNCTION()
		void ToPlayerRotate();

	//�m�b�N�o�b�N���郂�[�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AniMontage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AnimMontage_BossKnockMontage;		//�����F�ϐ�������AnimMontage_�͕K�{�炵��

	//�U�����鉼���[�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AniMontage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AnimMontage_BossAttack1;

	//�Ռ��g�U�����鉼���[�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AniMontage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AnimMontage_BossAttack2;

	//�O���ړ�
	UFUNCTION()
		void BossMove(float Speed , FVector MoveSize);

	//����ړ�
	UFUNCTION()
		void BackMove(float Speed);
};
