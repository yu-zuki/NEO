// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Bullet.generated.h"

UCLASS()
class NEO_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();
	// Sphere collision component.
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, Category = "Movement")
		float Speed = 1000.0f; // Adjust the speed as needed
	UPROPERTY(EditAnywhere, Category = "Movement")
		float DistanceSpeedDamping = 0.001f;

	FVector MovementDirection;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
