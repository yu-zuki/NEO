// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NEO/AnimationSystem/CharacterAnimNotifyState.h"
#include "EnemyAnimNotifyState.generated.h"

UCLASS()
class NEO_API UEnemyAnimNotifyState : public UCharacterAnimNotifyState
{
	GENERATED_BODY()
	
    // 通知をエディタに公開するかどうか
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const { return false; }


protected:

    // 通知開始時に行う処理
    virtual void NotifyBeginAction(AActor* _Owner);

    // 通知終了時に行う処理
    virtual void NotifyEndAction(AActor* _Owner);
};


UCLASS()
class NEO_API USwitchEnemyCollision : public UEnemyAnimNotifyState
{
    GENERATED_BODY()

    // 通知をエディタに公開するかどうか
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const { return true; }
};
