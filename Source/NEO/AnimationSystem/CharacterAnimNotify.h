// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CharacterAnimNotify.generated.h"

UCLASS()
class NEO_API UCharacterAnimNotify : public UAnimNotify
{
    GENERATED_BODY()


public:

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

    // このクラスはエディタに出さない
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const { return false; }


protected:

    AActor* GetOwnerActor(USkeletalMeshComponent* MeshComp);
    virtual void NotifyAction(AActor* _Owner);
};

