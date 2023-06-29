// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.h"
#include "Bullet.h"
#include "GunMan.generated.h"

UCLASS()
class NEO_API AGunMan : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGunMan();
	UPROPERTY(EditAnywhere, Category = "AI")
		float DesiredDistance = 500.0f; // プレイヤーからの望ましい距離（3m）

	UPROPERTY(EditAnywhere, Category = "AI")
		float MovementSpeed = 100.0f; // 移動速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* Attack;
	UPROPERTY()
		class ACharacter* PlayerCharacter; // プレイヤーキャラクターの参照
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	float Health;
	float MaxHealth = 120;
	int32 CallTracker = 5;
	UFUNCTION()
	void SpawnBullet();
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<class ABullet> BulletClass;

	FTimerHandle TimerHandle;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	FVector GetPlayerDirection() const;
	
	float GetDistanceToPlayer() const;
	void FacePlayer();
};
