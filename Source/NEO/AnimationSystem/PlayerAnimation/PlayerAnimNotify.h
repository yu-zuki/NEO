// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "NEO/AnimationSystem/CharacterAnimNotify.h"
#include "PlayerAnimNotify.generated.h"

UENUM(BlueprintType)
enum class EPlayerNotifyType :uint8
{
    NotifyType_SetCollision UMETA(DisplayName = "SetCollision"),
    NotifyType_SaveCombo UMETA(DisplayName = "SaveCombo"),
    NotifyType_ResetCombo UMETA(DisplayName = "ResetCombo"),
    NotifyType_CanControl UMETA(DisplayName = "CanControl"),
    NotifyType_SlowDown UMETA(DisplayName = "SlowDown")
};


UCLASS()
class NEO_API UPlayerAnimNotify : public UCharacterAnimNotify
{
    GENERATED_BODY()


public:

    // 通知をエディタに公開するかどうか
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const { return true; }

protected:
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AnimNotify")
        EPlayerNotifyType NotifyType;

    virtual void NotifyAction(AActor* _Owner);
};