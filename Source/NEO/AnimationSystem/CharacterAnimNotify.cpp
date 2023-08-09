// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimNotify.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

void UCharacterAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// �ʒm���������̏���
	NotifyAction(GetOwnerActor(MeshComp));
}

AActor* UCharacterAnimNotify::GetOwnerActor(USkeletalMeshComponent* MeshComp)
{
	// �v���C���[�̃x�[�X�N���X�擾
	AActor* pOwner = Cast<AActor>(MeshComp->GetOwner());
	if (!pOwner) { return nullptr; }

	return pOwner;
}

void UCharacterAnimNotify::NotifyAction(AActor* _Owner)
{
	return;
}