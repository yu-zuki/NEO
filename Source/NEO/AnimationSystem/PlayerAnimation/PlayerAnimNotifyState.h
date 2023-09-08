// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NEO/AnimationSystem/CharacterAnimNotifyState.h"
#include "PlayerAnimNotifyState.generated.h"


UENUM(BlueprintType)
enum class EPlayerNotifyStateType :uint8
{
    NotifyType_RootMotion_Attack1   UMETA(DisplayName = "RootMotion_Attack1"),
    NotifyType_RootMotion_Attack2   UMETA(DisplayName = "RootMotion_Attack2"),
    NotifyType_RootMotion_Attack3   UMETA(DisplayName = "RootMotion_Attack3"),
    NotifyType_RootMotion_KnockBack   UMETA(DisplayName = "RootMotion_KnockBack")
};

UCLASS()
class NEO_API UPlayerAnimNotifyState : public UCharacterAnimNotifyState
{
	GENERATED_BODY()
	
        // �ʒm���G�f�B�^�Ɍ��J���邩�ǂ���
        virtual bool CanBePlaced(UAnimSequenceBase* Animation) const { return true; }


protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
        EPlayerNotifyStateType NotifyType;

    // �ʒm�J�n���ɍs������
    virtual void NotifyBeginAction(AActor* _Owner, UAnimSequenceBase* Animation, float TotalDuration);

    // �ʒm�I�����ɍs������
    virtual void NotifyEndAction(AActor* _Owner, UAnimSequenceBase* Animation);
};
