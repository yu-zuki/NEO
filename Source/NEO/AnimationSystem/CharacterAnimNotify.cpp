// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimNotify.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

void UCharacterAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// �ʒm���������̏���
	NotifyAction(MeshComp);
}

void UCharacterAnimNotify::NotifyAction(USkeletalMeshComponent* MeshComp)
{
	return;
}