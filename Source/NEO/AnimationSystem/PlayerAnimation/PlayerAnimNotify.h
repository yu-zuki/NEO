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

    // �ʒm���G�f�B�^�Ɍ��J���邩�ǂ���
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const { return false; }

protected:

    virtual void NotifyAction(AActor* _Owner);
};


//--------�����蔻��ʒm-----------------------------
UCLASS()
class NEO_API USetCollision : public UPlayerAnimNotify
{
    GENERATED_BODY()


public:

    // �ʒm���G�f�B�^�Ɍ��J���邩�ǂ���
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const
    {
        // �����^�[�W���̃G�f�B�^�ł̂݌��J
        if (Animation->IsA(UAnimMontage::StaticClass())) { return true; }

        return false;
    }
};


//--------�R���{�p���ʒm------------------------------
UCLASS()
class NEO_API USaveCombo : public UPlayerAnimNotify
{
    GENERATED_BODY()


public:

    // �ʒm���G�f�B�^�Ɍ��J���邩�ǂ���
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const
    {
        // �����^�[�W���̃G�f�B�^�ł̂݌��J
        if (Animation->IsA(UAnimMontage::StaticClass())) { return true; }

        return false;
    }
};


//--------�R���{�I���ʒm------------------------------
UCLASS()
class NEO_API UResetCombo : public UPlayerAnimNotify
{
    GENERATED_BODY()


public:

    // �ʒm���G�f�B�^�Ɍ��J���邩�ǂ���
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const
    {
        // �����^�[�W���̃G�f�B�^�ł̂݌��J
        if (Animation->IsA(UAnimMontage::StaticClass())) { return true; }

        return false;
    }
};


//--------����\�ɂȂ�ʒm------------------------------
UCLASS()
class NEO_API UCanControl : public UPlayerAnimNotify
{
    GENERATED_BODY()


public:

    // �ʒm���G�f�B�^�Ɍ��J���邩�ǂ���
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const
    {
        // �����^�[�W���̃G�f�B�^�ł̂݌��J
        if (Animation->IsA(UAnimMontage::StaticClass())) { return true; }

        return false;
    }
};

//--------�A�j���[�V������x������ʒm----------------------
UCLASS()
class NEO_API USlowDown : public UPlayerAnimNotify
{
    GENERATED_BODY()


public:

    // �ʒm���G�f�B�^�Ɍ��J���邩�ǂ���
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const
    {
        // �����^�[�W���̃G�f�B�^�ł̂݌��J
        if (Animation->IsA(UAnimMontage::StaticClass())) { return true; }

        return false;
    }
};
