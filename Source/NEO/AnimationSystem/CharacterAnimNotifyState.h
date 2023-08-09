// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CharacterAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class NEO_API UCharacterAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);

    virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);

    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

    // �ʒm���G�f�B�^�Ɍ��J���邩�ǂ���
    virtual bool CanBePlaced(UAnimSequenceBase* Animation) const { return false; }


protected:

    // �ʒm�J�n���ɍs������(�h����̃N���X�ŃI�[�o�[���C�h)
    virtual void NotifyBeginAction(AActor* _Owner) { return; }

    // �ʒm����ɍs������(�h����̃N���X�ŃI�[�o�[���C�h)
    virtual void NotifyTickAction(AActor* _Owner) { return; }

    // �ʒm�I�����ɍs������(�h����̃N���X�ŃI�[�o�[���C�h)
    virtual void NotifyEndAction(AActor* _Owner) { return; }

private:

    // �I�[�i�[�̃A�N�^�[���擾
    AActor* GetOwnerActor(USkeletalMeshComponent* MeshComp);	
};
