// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPPOdaEnum.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"			//�L�����N�^�[���[�u�����g���g�����߂ɕK�v�ȃw�b�_�[
#include "Kismet/GameplayStatics.h"								//getplayerpawn�����g�����߂ɕK�v�ȃw�b�_�[
#include "Kismet/KismetSystemLibrary.h"							//printString���g�����߂ɕK�v�ȃw�b�_�[
#include "Animation/AnimMontage.h"								//�A�j���[�V�����𗬂����߂ɕK�v�ȃw�b�_�[
#include "Math/UnrealMathUtility.h"								//RandRange���g�����߂ɕK�v�ȃw�b�_�[
#include "Math/Vector.h"										//Dist(����)���g�����߂ɕK�v�ȃw�b�_�[
#include "Components/BoxComponent.h"							//�{�b�N�X�R���|�[�l���g���g������
#include "Templates/SubclassOf.h"								//TSubclassOf���g�����߂ɕK�v�ȃw�b�_�[
#include "PlayerSystem/PlayerCharacter.h"						//�v���C���[�L�������擾���邽�߂ɕK�v�ȃw�b�_�[
#include "NiagaraComponent.h"									//�i�C�A�K���G�t�F�N�g���ĂԂ��߂ɕK�v�ȃw�b�_�[
#include "NiagaraFunctionLibrary.h"								//�i�C�A�K���G�t�F�N�g���ĂԂ��߂ɕK�v�ȃw�b�_�[
#include "NEO/PlayerSystem/ActionAssistComponent.h"

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

	//�֐�-------------------------------------------------------------------------------------------------------------------

	//�{�X�̃X�e�[�g�ł̏���----------------
	void OdaStay1(int Timer);

	void OdaAttack1(int Timer);

	void OdaAttack2(int Timer);

	void OdaUlt(int Timer);
	//------------------------------------

	//�{�X�̓����̏���
	void OdaMove1(int DeltaTime, int StopTimer);


	//�X�|�[��������--------------------------------------------------------------------------
	//�X�|�[�����̎~�܂�Ƃ���
	UPROPERTY()
		bool SpawnDelay;

	//�X�|�[�����~�܂鎞��
	UPROPERTY()
		int SpawnTimer;
	//----------------------------------------------------------------------------------------


	//�J�E���^�[------------------------------------------------------------
	//���Ԃ̎擾
	int FlameCounter;

	//int�^�̃J�E���^�[
	int WaitTime;
	//---------------------------------------------------------------------

	//�{�X�������Ă������ǂ���
	UPROPERTY(BlueprintReadOnly)
	bool isMove;

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

	//�񋓌^
	ECPPOdaEnum OdaMoveEnum;

	//�ߐڍU�����������ăv���C���[���悯��Ȃ��̂ŏ����x��������
	UPROPERTY()
		int Attack1Delay;

	//random�̒l������ׂ̕ϐ�
	UPROPERTY()
		int RandomNum;

public:

	//���̏����擾
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sword", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* swordConp;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//�R���|�[�l���g-----------------------------------------------------------------------------------------------------
	//�V�[���R���|�[�l���g
	UPROPERTY()
		USceneComponent* Parent;

	//�v���C���[�L�����N�^�[
	UPROPERTY()
		AActor* PlayerChara;

	//���̃R���W����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BoxComp", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxComp;

	//�L�����N�^�[���[�u�����g
	UPROPERTY()
		UCharacterMovementComponent* NobunagaMovement;

	UPROPERTY()
		class UActionAssistComponent* ActionAssistComp;
	//--------------------------------------------------------------------------------------------------------------------


	//�_���[�W����-----------------------------------------------------
	UFUNCTION()
		virtual void ApplyDamage(float Damage);


	UFUNCTION()
		void WorldTimeReturn();

	//�_���[�W�^�C�v�N���X
	UPROPERTY()
		TSubclassOf < class UDamageType > DamageTypeClass;

	//�Ռ��g�̃_���[�W�����邽�߂̕ϐ�
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
		float BladeDamage;

	//�ʏ�̏Ռ��g�̃_���[�W
	UPROPERTY(EditAnywhere, Category = "Damage")
		float ShockWaveDamage;

	//�K�E�Z���̏Ռ��g�̃_���[�W
	UPROPERTY(EditAnywhere, Category = "Damage")
		float UltShockWaveDamage;

	//�K�E�Z�̃_���[�W
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
		float UltDamage;
	//----------------------------------------------------------------




	//���x----------------------------------
	//�ʏ�̈ړ����x
	UPROPERTY(EditAnywhere)
		float OdaSpeed;
	//--------------------------------------



	//�Ռ��g-----------------------------------------------------------------------------
	//�������Ռ��g���o�����߂̕ϐ�
	UPROPERTY()
		bool OneMoreShockWave;

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
	//-----------------------------------------------------------------------------------

	//�K�E�Z---------------------------------------------------------------------------
	//�K�E�Z�̏o���^�C�~���O�̒���
	UPROPERTY()
		bool isUltSpawnTiming;

	//�K�E�Z��ł^�C�~���O�̒���
	UPROPERTY(BlueprintReadOnly)
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

	//�K�E�Z���������ł��߂̃J�E���^�[
	UPROPERTY()
		int UltTimer;

	//------------------------------------------------------------------------------

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

	//�q�b�g�G�t�F�N�g�ϐ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		UNiagaraSystem* HitParticles;

	//HP���A���Ō���Ȃ��悤�Ƀ��b�N
	UPROPERTY()
		bool isHPLock;

	//HP���A���Ō���Ȃ��悤�Ƀ��b�N���邽�߂̊֐�
	UFUNCTION()
		void HPLock();

	//�v���C���[�̕����Ɍ����鏈��
	UFUNCTION()
		void ToPlayerRotate();

	//�ߋ����U���̃R���{
	UFUNCTION()
		void Is1Combo();

	//�������U���̃R���{
	UFUNCTION()
		void Is2Combo();

	//�R���{�������ǂ����őҋ@���Ԃ��ς��̂Ń^�C�}�[��p��
	UPROPERTY()
		int Attack1WaitTimer;

	//�ߐڂ̑ҋ@����
	UPROPERTY(EditAnywhere)
		int Attack1WaitingTime;

	//�ҋ@���邩�ǂ���
	bool isAttack1Waiting;

	//�ҋ@���邽�߂̊֐�
	UFUNCTION()
		void Attack1Wait();

	//�ߐڃR���{�����i�ڂ�
	UPROPERTY()
		int Combo1Counter;

	//�������R���{�����i�ڂ�
	UPROPERTY()
		int Combo2Counter;

	//�_���[�W���Z�����l��ۑ����Ă����ϐ�
	float HoldDamageAdd;

	//�v���C���[���m�b�N�o�b�N�����邽�߂ɍŌ�̍U�����ǂ������Ƃ�֐�
	UFUNCTION(BlueprintCallable)
		bool LastAttack();

	UPROPERTY()
		bool isBossHPRock;

	UFUNCTION()
		void BossHPRock();

	//�񋓌^��Stay�ɖ߂��ׂ̊֐�
	UFUNCTION()
		void BacktoStayCase();


	//���[�V����--------------------------------------------------------------------------------------------------
	//�̂����郂�[�V����
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

	//�ӂ��Ƃԃ��[�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AniMontage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AnimMontage_BossBlowAway;

	//���S���[�V����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AniMontage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AnimMontage_BossDeath;
	//-------------------------------------------------------------------------------------------------------------

	//�m�b�N�o�b�N���Ăӂ��ƂԊ֐�
	UFUNCTION()
		void BossKnockback();

	//�A�j���[�V�������ɒʒm�ŌĂяo�����S�֐�
	UFUNCTION()
		void  DeathMotion();

	FTimerHandle TimerHandle_DeathToGameOver;

	FTimerHandle TimerHandle_KnockbackAnimationFinish;

	//�O���ړ�
	UFUNCTION()
		void BossMove(float Speed, FVector MoveSize);

	//�_���[�W�l
	UPROPERTY(EditAnywhere, Category = "Damage")
		float SwordFirstDamage;

	UPROPERTY(EditAnywhere, Category = "Damage")
		float SwordAddDamage;

	//�G���G���Ăԏ���
	UFUNCTION(BlueprintCallable)
		void CallEnemy(FString path , FVector Location);

	//�G���G���Ăԏ�������񂾂��������ׂ̕ϐ�
	int EnemyOneTimeSpawn;

	//�e�������G�̃p�X
	FString GunPath = "/Game/0102/Enemy/Gunman/MyGunMan3.MyGunMan3_C"; // /Content �ȉ��̃p�X�� /Game �ȉ��̃p�X�ɒu�������

	//��莝�����G�̃p�X
	FString LancePath = "/Game/0102/Enemy/Lancer/BP_LancerTag3.BP_LancerTag3_C"; // /Content �ȉ��̃p�X�� /Game �ȉ��̃p�X�ɒu�������

	//���������G�̃p�X
	FString SwordPath = "/Game/0102/Enemy/Soldier/BP_SoldierTag3.BP_SoldierTag3_C"; // /Content �ȉ��̃p�X�� /Game �ȉ��̃p�X�ɒu�������

	//�{�b�N�X�R���|�[�l���g�̃I�[�o�[���b�v����
	UFUNCTION()
		void CheckOverlap();

	//�v���C���[----------------------------------------------------------------------
	//�v���C���[�ɓ���������
	UFUNCTION()
		void PlayerOnOverlap(FHitResult& _HitResult);

	//�v���C���[�ɑ΂��Ă�HP���b�N
	UPROPERTY()
		bool bIsAttacked;

	//�v���C���[�ɑ΂��Ă�HP���b�N���邽�߂̊֐�
	UFUNCTION()
		void toPlayerAttacked();
	//-----------------------------------------------------------------------------------


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

	UFUNCTION(BlueprintCallable)
		float GetHP()const { return Health; }

	//�̗�
	UPROPERTY(EditAnywhere)
		float Health;

	UPROPERTY(EditAnywhere)
		float MaxHealth;

protected:
	AActor* GetPlayer();
};
