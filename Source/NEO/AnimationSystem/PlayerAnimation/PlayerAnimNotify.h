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

    // 通知をエディタに公開するかどうか
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const { return false; }

protected:

    virtual void NotifyAction(AActor* _Owner);
};


//--------当たり判定通知-----------------------------
UCLASS()
class NEO_API USetCollision : public UPlayerAnimNotify
{
    GENERATED_BODY()


public:

    // 通知をエディタに公開するかどうか
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const
    {
        // モンタージュのエディタでのみ公開
        if (Animation->IsA(UAnimMontage::StaticClass())) { return true; }

        return false;
    }
};


//--------コンボ継続通知------------------------------
UCLASS()
class NEO_API USaveCombo : public UPlayerAnimNotify
{
    GENERATED_BODY()


public:

    // 通知をエディタに公開するかどうか
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const
    {
        // モンタージュのエディタでのみ公開
        if (Animation->IsA(UAnimMontage::StaticClass())) { return true; }

        return false;
    }
};


//--------コンボ終了通知------------------------------
UCLASS()
class NEO_API UResetCombo : public UPlayerAnimNotify
{
    GENERATED_BODY()


public:

    // 通知をエディタに公開するかどうか
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const
    {
        // モンタージュのエディタでのみ公開
        if (Animation->IsA(UAnimMontage::StaticClass())) { return true; }

        return false;
    }
};


//--------操作可能になる通知------------------------------
UCLASS()
class NEO_API UCanControl : public UPlayerAnimNotify
{
    GENERATED_BODY()


public:

    // 通知をエディタに公開するかどうか
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const
    {
        // モンタージュのエディタでのみ公開
        if (Animation->IsA(UAnimMontage::StaticClass())) { return true; }

        return false;
    }
};

//--------アニメーションを遅くする通知----------------------
UCLASS()
class NEO_API USlowDown : public UPlayerAnimNotify
{
    GENERATED_BODY()


public:

    // 通知をエディタに公開するかどうか
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const
    {
        // モンタージュのエディタでのみ公開
        if (Animation->IsA(UAnimMontage::StaticClass())) { return true; }

        return false;
    }
};
