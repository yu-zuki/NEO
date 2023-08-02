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
#include "Math/Vector.h"				//Dist(����)���g�����߂ɕK�v�ȃw�b�_�[
#include "Components/BoxComponent.h"
#include "Engine/EngineTypes.h"
#include "Components/StaticMeshComponent.h"
#include "Templates/SubclassOf.h"
#include "PlayerSystem/PlayerCharacter.h"

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

	//�v���C���[�L�����N�^�[
	UPROPERTY()
		AActor* PlayerChara;

	//���̃R���W����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BoxComp", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;

	//�L�����N�^�[���[�u�����g
	UPROPERTY()
		UCharacterMovementComponent* NobunagaMovement;

	UPROPERTY()
		bool SpawnDelay;

	UPROPERTY()
		int SpawnTimer;

	//�_���[�W����-----------------------------------------------------
	UFUNCTION()
	virtual void ApplyDamage(float Damage);

	//�_���[�W�^�C�v�N���X
	UPROPERTY()
		TSubclassOf < class UDamageType > DamageTypeClass;
	//----------------------------------------------------------------

	//���Ԃ̎擾
	int FlameCounter;

	//�񋓌^
	ECPPOdaEnum OdaMoveEnum;

	//�ҋ@�֐�
	UPROPERTY()
	bool SwitchStayMove;

	//int�^�̃J�E���^�[
	int WaitTime;

	//�{�X�ƃv���C���[�Ƃ̋���----------------------------
	//X��
	//�{�X
	FVector BossPosX;
	//�v���C���[
	FVector PlayerPosX;
	//Y��
	//�{�X
	FVector BossPosY;
	//�v���C���[
	FVector PlayerPosY;
	//---------------------------------------------------


	//�{�X�̃X�e�[�g�ł̏���----------------
	void OdaStay1(int Timer);

	void OdaBack1(int Timer);

	void OdaAttack1(int Timer);

	void OdaAttack2(int Timer);

	void OdaUlt(int Timer);
	//------------------------------------

	//���x----------------------------------
	//�ʏ�̈ړ����x
	UPROPERTY(EditAnywhere)
		float OdaSpeed;

	//�}�ȑ��x
	UPROPERTY(EditAnywhere)
		float FastOdaSpeed;
	//--------------------------------------

	//�ߐڍU�����������ăv���C���[���悯��Ȃ��̂ŏ����x��������
	UPROPERTY()
	int Attack1Delay;

	void OdaMove1(int DeltaTime, int StopTimer);

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

	//�K�E�Z�̏o���^�C�~���O�̒���
	UPROPERTY()
		bool isUltSpawnTiming;

	//�K�E�Z��ł^�C�~���O�̒���
	UPROPERTY()
		bool isUltShotTiming;

	//�K�E�Z�̏o���^�C�~���O�̒������邽�߂̊֐�
	UFUNCTION()
		void UltSpawnFlagChange();

	//�K�E�Z���o��������ׂ̕ϐ�
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor>UltSpawn;

	//�K�E�Z�͈�񂾂��o�������̂Ńt���O�ŊǗ�
	UPROPERTY()
		bool isUltShot;

	UPROPERTY()
		int UltTimer;

	//�U���̃t���O
	UPROPERTY(BlueprintReadOnly)
		bool IsAttackNow;

	//�U���̃t���O��؂�ւ���ׂ̊֐�
	UFUNCTION()
		void AttackFlagChange();

	//�U�����ꂽ�񐔂���萔��������ߐڍU�����ςȂ��ׂ̕ϐ�
	UPROPERTY()
		bool isNotAttackNow;
	//��萔�̌v��
	UPROPERTY()
		int NotAttackCount;



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

	//�Ռ��g�U�����鉼���[�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AniMontage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AnimMontage_BossUltimate;

	//�O���ړ�
	UFUNCTION()
		void BossMove(float Speed , FVector MoveSize);

	//����ړ�
	UFUNCTION()
		void BackMove(float Speed);

	//�_���[�W�l
	UPROPERTY(EditAnywhere, Category = "Damage")
		float SwordDamage;

	UFUNCTION()
		void CheckOverlap();

	UFUNCTION()
		void EnemyOnOverlap(FHitResult& _HitResult);

	UPROPERTY()
		bool bIsAttacked;

	UFUNCTION()
		void toPlayerAttacked();

//////////////////////////////////////////////////////////////////////////
//���S����
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Area")
		bool IsAreaEnemy = false;

	void Death();

//////////////////////////////////////////////////////////////////////////
//UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		class UEnemyBase_WidgetComponent* EnemyWidget;

	//�̗�
	UPROPERTY(EditAnywhere)
		float Health;

	UPROPERTY(EditAnywhere)
		float MaxHealth;
};
