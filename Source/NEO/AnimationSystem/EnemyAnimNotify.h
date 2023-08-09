// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAnimNotify.h"
#include "EnemyAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class NEO_API UEnemyAnimNotify : public UCharacterAnimNotify
{
	GENERATED_BODY()
	
public:

	virtual bool CanBePlaced(UAnimSequenceBase* Animation) const { return false; }

protected:

	virtual void NotifyAction(AActor* _Owner);
};

UCLASS()
class NEO_API UEnemyCollisionOn : public UEnemyAnimNotify
{
	GENERATED_BODY()

public:

	virtual bool CanBePlaced(UAnimSequenceBase* Animation) const { return true; }
};
UCLASS()
class NEO_API UEnemyCollisionOff : public UEnemyAnimNotify
{
	GENERATED_BODY()

public:

	virtual bool CanBePlaced(UAnimSequenceBase* Animation) const { return true; }
};
