// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Rolling.generated.h"

UCLASS()
class NEO_API ARolling : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARolling();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sphere")
		UStaticMeshComponent* Sphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
		USphereComponent* RollingSphere;
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

};
