// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CurveTrigger.generated.h"

UCLASS()
class NEO_API ACurveTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACurveTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// ÉvÉåÉCÉÑÅ[Çã»Ç™ÇËäpÇ≈ã»Ç∞ÇÈ
	void SetPlayerMoveAngle();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void OnActorOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		virtual void OnActorOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);



private:
	class UBoxComponent* CurveTriggerComp;
};
