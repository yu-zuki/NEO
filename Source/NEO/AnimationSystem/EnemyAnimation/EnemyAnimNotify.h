// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NEO/AnimationSystem/CharacterAnimNotify.h"
#include "EnemyAnimNotify.generated.h"


UENUM(BlueprintType)
enum class EEnemyNotifyType :uint8
{
	NotifyType1 UMETA(DisplayName = "EnemyCollisionOn"),
	NotifyType2 UMETA(DisplayName = "EnemyCollisionOff"),
	NotifyType3 UMETA(DisplayName = "SpawnTrajectoryBullet"),
	NotifyType4 UMETA(DisplayName = "SpawnBullet")
};

UCLASS()
class NEO_API UEnemyAnimNotify : public UCharacterAnimNotify
{
	GENERATED_BODY()
	
public:

	virtual bool CanBePlaced(UAnimSequenceBase* Animation) const { return true; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
		EEnemyNotifyType NotifyType;

	virtual void NotifyAction(AActor* _Owner, UAnimSequenceBase* Animation);
};

