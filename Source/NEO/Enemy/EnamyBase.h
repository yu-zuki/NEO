// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DeathTrigger.h"
#include "NEO/GameSystem/GameSystem_BattleArea.h"
#include "NiagaraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "EnamyBase.generated.h"

UCLASS()
class NEO_API AEnamyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnamyBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Area")
	bool IsAreaEnemy = false;
	//�A�j���[�V����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* Attack2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* Attack3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* Death;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* Damage_Reaction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* MovingAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wepon")
		UStaticMeshComponent* Wepon;
	UPROPERTY()
		UAnimInstance* AnimInstance;
	UFUNCTION(BlueprintCallable, Category = "Area")
	virtual void DestoryEnemy();
	UPROPERTY()
		class ACharacter* PlayerCharacter; // �v���C���[�L�����N�^�[�̎Q��
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UPROPERTY(VisibleAnywhere, Category = "Collision")
		class UBoxComponent* DamageCollision;
	
	// �U���̃A�V�X�g�p�R���|�[�l���g
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AttackAssist, meta = (AllowPrivateAccess = "true"))
		class UActionAssistComponent* ActionAssistComp;
	// Damage to be dealt to the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float Damage;
	UPROPERTY(VisibleAnywhere)
		USplineComponent* MoveSpline;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY()
		FTimerHandle TimerHandle_DestroyEnemy;
	AActor* GetPlayer();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* NiagaraEffect;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float  Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float  MaxHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
		TSubclassOf<ADeathTrigger>DeathTriggerClass;
	
	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual void ApplyDamage(float DamageAmount);

	void MaintainDistanceFromEnemy();
	 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy", meta = (AllowPrivateAccess = "true"))
		float DesiredDistanceFromEnemy = 300.0f;

	bool bHasPattern1Tag; // pattern1�Ƃ���Tag�������Ă��邩�ǂ���
	bool bHasPattern2Tag;
	bool bHasPattern3Tag;

	void AfterDeath();

	void DamageReac();

	bool bIsNowDamage;

	void CheckHealth();

	void SpawnDeathTrigger();

	void CheckCollisonOn();

	void CheckCollisonOff();

	bool bIsShoot = false;

	UFUNCTION(BlueprintCallable)
		float GetHP()const { return Health; }
	
	bool bIsRotation;

	bool bIsRotationTag2;

	bool bIsDeath;
	
	bool IsAnimationAttacking() const;
	UFUNCTION(BlueprintCallable)
	bool IsIdol() const { return bIsIdol; }
	bool IsWalking() const { return bIsWalking; }
	UFUNCTION(BlueprintCallable)
	bool IsRunning ()const { return bIsRunning; }
	bool IsAttacking() const { return bIsAttacking; }
	bool IsReactingToDamage() const { return bIsReactingToDamage; }
	bool IsDead() const { return bIsDead; }
	bool IsAir() const{ return bIsAir; }


	bool bIsIdol;
	bool bIsWalking;
	bool bIsRunning;
	bool bIsAttacking;
	bool bIsReactingToDamage;
	bool bIsDead;
	bool bIsAir;
	///UI
public:

	AActor* GetEnemyActor() const;
	TArray<AGameSystem_BattleArea*> BattleAreaReferences;  // �����̃o�g���G���A�̎Q��
	float SpawnTime; // �X�|�[���������Ԃ��L�^����ϐ�
	USplineComponent* MovementSpline;

	// �ړ����J�n���邽�߂̃t���O�Ǝ���
	bool bShouldMoveAlongSpline = false;
	float TimeSinceStartOfMovement = 0.0f;

	// 3�b�Ԉړ����邽�߂̒萔
	static constexpr float MovementDuration = 3.0f;
	ECollisionChannel EnemyCollison = ECollisionChannel::ECC_GameTraceChannel1;
	
	UFUNCTION()
    void OnOverlapBeginWall(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};