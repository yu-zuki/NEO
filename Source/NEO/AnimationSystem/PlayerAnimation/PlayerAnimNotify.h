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
    NotifyType_ResetCombo   UMETA(DisplayName = "ResetCombo"),
    NotifyType_SaveCombo    UMETA(DisplayName = "SaveCombo"),
    NotifyType_CanControl   UMETA(DisplayName = "CanControl"),
    NotifyType_SlowDown     UMETA(DisplayName = "SlowDown")
};


UCLASS()
class NEO_API UPlayerAnimNotify : public UCharacterAnimNotify
{
    GENERATED_BODY()

        
public:

    // �ʒm���G�f�B�^�Ɍ��J���邩�ǂ���
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const;

protected:

    // �ʒm�����Ǘ�����Enum
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AnimNotify")
        EPlayerNotifyType NotifyType;

    // �ʒm���������̏���
    virtual void NotifyAction(AActor* _Owner,UAnimSequenceBase* Animation);
};