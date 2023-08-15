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

    // �ʒm�������Ƃ��ɍs������
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

    // �ʒm���G�f�B�^�Ɍ��J���邩�ǂ���
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const { return false; }


protected:

    // �ʒm���������ɍs������(�h����̃N���X�ŃI�[�o�[���C�h)
    virtual void NotifyAction(AActor* _Owner, UAnimSequenceBase* Animation) { return; }

private:

    // �I�[�i�[�̃A�N�^�[���擾
    AActor* GetOwnerActor(USkeletalMeshComponent* MeshComp);
};

