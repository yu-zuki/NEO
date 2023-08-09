// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimNotify.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

void UCharacterAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// オーナー取得
	AActor* OwnerActor = GetOwnerActor(MeshComp);
	if (!OwnerActor) { return; }


	// 通知が来た時の処理
	NotifyAction(OwnerActor);
}

AActor* UCharacterAnimNotify::GetOwnerActor(USkeletalMeshComponent* MeshComp)
{
	// プレイヤーのベースクラス取得
	AActor* pOwner = Cast<AActor>(MeshComp->GetOwner());
	if (!pOwner) { return nullptr; }

	return pOwner;
}