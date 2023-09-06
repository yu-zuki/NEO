// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnamyBase.h"
#include "Rolling.h"
#include "Pusher.generated.h"

UCLASS()
class NEO_API APusher : public AEnamyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APusher();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* Push;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* Idol;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* DeathMon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		TSubclassOf<ARolling> RollingToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float  HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float  MaxHP;
	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual void ApplyDamagePush(float DamageAmount);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY()
		FTimerHandle TimerHandle_DeathPusher;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SpawnRolling();

	FTimerHandle RollingSpawnTimer;
	UFUNCTION()
		void DetachRolling();

	void DeathPusher();

	ARolling* CurrentRolling;

	bool DeathCheck;
};
