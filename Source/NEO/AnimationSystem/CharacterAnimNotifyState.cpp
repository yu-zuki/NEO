// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimNotifyState.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

void UCharacterAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	// �I�[�i�[�擾
	AActor* OwnerActor = GetOwnerActor(MeshComp);
	if (!OwnerActor) { return; }


	// �ʒm�J�n���̏���
	NotifyBeginAction(OwnerActor);
}


void UCharacterAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	// �I�[�i�[�擾
	AActor* OwnerActor = GetOwnerActor(MeshComp);
	if (!OwnerActor) { return; }


	// �ʒm����ɏ���
	NotifyTickAction(OwnerActor);
}



void UCharacterAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// �I�[�i�[�擾
	AActor* OwnerActor = GetOwnerActor(MeshComp);
	if (!OwnerActor) { return; }


	// �ʒm�I�����̏���
	NotifyEndAction(OwnerActor);
}

// �I�[�i�[�̃A�N�^�[���擾
AActor* UCharacterAnimNotifyState::GetOwnerActor(USkeletalMeshComponent* MeshComp)
{
	// �v���C���[�̃x�[�X�N���X�擾
	AActor* pOwner = Cast<AActor>(MeshComp->GetOwner());
	if (!pOwner) { return nullptr; }

	return pOwner;
}