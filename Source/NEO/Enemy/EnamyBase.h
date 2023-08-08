// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DeathTrigger.h"
#include "NiagaraComponent.h"
#include "EnemyBaseAnimInstance.h"
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
	//アニメーション
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* Death;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* Damage_Reaction;
	UFUNCTION(BlueprintCallable, Category = "Area")
	virtual void DestoryEnemy();
	UPROPERTY()
		class ACharacter* PlayerCharacter; // プレイヤーキャラクターの参照
	
	// 攻撃のアシスト用コンポーネント
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AttackAssist, meta = (AllowPrivateAccess = "true"))
		class UActionAssistComponent* ActionAssistComp;

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


	void AfterDeath();

	void DamageReac();

	bool bIsNowDamage;

	void CheckHealth();

	void SpawnDeathTrigger();

	bool bIsShoot = false;

	float GetHP() { return Health; }
	
	bool bIsRotation;

	bool bIsDeath;
	
	bool IsIdol() const { return bIsIdol; }
	bool IsWalking() const { return bIsWalking; }
	bool IsRunning ()const { return bIsRunning; }
	bool IsAttacking() const { return bIsAttacking; }
	bool IsReactingToDamage() const { return bIsReactingToDamage; }
	bool IsDead() const { return bIsDead; }
	bool IsAir() const{ return bIsAir; }
private:

	///////////////////////////////アニメーション///////////////////////////////////////////
	bool bIsIdol;
	bool bIsWalking;
	bool bIsRunning;
	bool bIsAttacking;
	bool bIsReactingToDamage;
	bool bIsDead;
	bool bIsAir;
	///UI
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		class UEnemyBase_WidgetComponent* EnemyWidget;
	AActor* GetEnemyActor() const;
};