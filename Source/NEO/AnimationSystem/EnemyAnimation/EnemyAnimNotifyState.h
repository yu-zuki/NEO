// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NEO/AnimationSystem/CharacterAnimNotifyState.h"
#include "EnemyAnimNotifyState.generated.h"


UENUM(BlueprintType)
enum class EEnemyNotifyType2 :uint8
{
    NotifyType1 UMETA(DisplayName = "SwitchEnemyCollision"),
    NotifyType2 UMETA(DisplayName = "SpawnEffect"),
};

UCLASS()
class NEO_API UEnemyAnimNotifyState : public UCharacterAnimNotifyState
{
	GENERATED_BODY()
	
    // �ʒm���G�f�B�^�Ɍ��J���邩�ǂ���
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const { return true; }


protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
        EEnemyNotifyType2 NotifyType;

    // �ʒm�J�n���ɍs������
    virtual void NotifyBeginAction(AActor* _Owner);

    // �ʒm�I�����ɍs������
    virtual void NotifyEndAction(AActor* _Owner);
};
