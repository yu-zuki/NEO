// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NEO/AnimationSystem/CharacterAnimNotifyState.h"
#include "EnemyAnimNotifyState.generated.h"

UCLASS()
class NEO_API UEnemyAnimNotifyState : public UCharacterAnimNotifyState
{
	GENERATED_BODY()
	
    // �ʒm���G�f�B�^�Ɍ��J���邩�ǂ���
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const { return false; }


protected:

    // �ʒm�J�n���ɍs������
    virtual void NotifyBeginAction(AActor* _Owner);

    // �ʒm�I�����ɍs������
    virtual void NotifyEndAction(AActor* _Owner);
};


UCLASS()
class NEO_API USwitchEnemyCollision : public UEnemyAnimNotifyState
{
    GENERATED_BODY()

    // �ʒm���G�f�B�^�Ɍ��J���邩�ǂ���
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const { return true; }
};
