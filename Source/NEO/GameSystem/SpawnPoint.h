// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "SpawnPoint.generated.h"

/**
 * 
 */
UCLASS()
class NEO_API ASpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

//protected:
	//Actor��Class
protected:
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AActor> ActorClass;
public:
	//Actor��Class��Ԃ�
	UFUNCTION(BlueprintCallable, Category = "Spawn")
		TSubclassOf<AActor> GetSpawnActorClass() const { return ActorClass; }
};
