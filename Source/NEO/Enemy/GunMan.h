// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NEO/PlayerSystem/PlayerCharacter.h"
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
		float MovementSpeed = 100.0f; // �ړ����x

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Rotation")
		bool bIsRotation;
	FVector GetPlayerDirection() const;
	
	float GetDistanceToPlayer() const;
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ATrajectoryBullet> TrajectoryBulletClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ABullet> BulletClass;

	FTimerHandle BulletSpawnTimerHandle;
	FTimerHandle MovementResumeTimerHandle;

	void SpawnTrajectoryBullet();
	void ReplaceWithBullet();
	void ResumeMovement();
};
