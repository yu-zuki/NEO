// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerSystem\PlayerCharacter.h"
#include "Bullet.h"
#include "TrajectoryBullet.h"
#include "EnamyBase.h"

#include "GunMan.generated.h"

UCLASS()
class NEO_API AGunMan : public AEnamyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGunMan();
	UPROPERTY(EditAnywhere, Category = "AI")
		float DesiredDistance = 500.0f; // プレイヤーからの望ましい距離（3m）

	UPROPERTY(EditAnywhere, Category = "AI")
		float MovementSpeed = 100.0f; // 移動速度

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	int32 CallTracker = 5;
	UFUNCTION()
	void SpawnBullet();
	UFUNCTION()
	void BlinkTrajectoryBullet();
	
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<class ABullet> BulletClass;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<class ATrajectoryBullet> TrajectoryBulletClass;
	UPROPERTY()
		FTimerHandle TimerHandle_Blink;

	UPROPERTY()
		FTimerHandle TimerHandle_Destroy;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Rotation")
		bool bIsRotation;
	FVector GetPlayerDirection() const;
	
	float GetDistanceToPlayer() const;
	void FacePlayer();
};
