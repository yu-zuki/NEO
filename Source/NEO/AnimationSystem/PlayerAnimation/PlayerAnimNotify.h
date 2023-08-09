// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "NEO/AnimationSystem/CharacterAnimNotify.h"
#include "PlayerAnimNotify.generated.h"

UCLASS()
class NEO_API UPlayerAnimNotify : public UCharacterAnimNotify
{
    GENERATED_BODY()


public:

    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const { return false; }

protected:

    virtual void NotifyAction(AActor* _Owner);
};


UCLASS()
class NEO_API UCollision : public UPlayerAnimNotify
{
    GENERATED_BODY()


public:

    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const
    {
        return true;
    }
};


UCLASS()
class NEO_API USaveCombo : public UPlayerAnimNotify
{
    GENERATED_BODY()


public:

    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const
    {
        return true;
    }
};

UCLASS()
class NEO_API UResetCombo : public UPlayerAnimNotify
{
    GENERATED_BODY()


public:

    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const
    {
        return true;
    }
};

UCLASS()
class NEO_API UCanControl : public UPlayerAnimNotify
{
    GENERATED_BODY()


public:

    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const
    {
        return true;
    }
};

UCLASS()
class NEO_API USlowDown : public UPlayerAnimNotify
{
    GENERATED_BODY()


public:

    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const
    {
        return true;
    }
};
