// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimNotifyState.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

void UCharacterAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	// オーナー取得
	AActor* OwnerActor = GetOwnerActor(MeshComp);
	if (!OwnerActor) { return; }


	// 通知開始時の処理
	NotifyBeginAction(OwnerActor);
}


void UCharacterAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	// オーナー取得
	AActor* OwnerActor = GetOwnerActor(MeshComp);
	if (!OwnerActor) { return; }


	// 通知中常に処理
	NotifyTickAction(OwnerActor);
}



void UCharacterAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// オーナー取得
	AActor* OwnerActor = GetOwnerActor(MeshComp);
	if (!OwnerActor) { return; }


	// 通知終了時の処理
	NotifyEndAction(OwnerActor);
}

// オーナーのアクターを取得
AActor* UCharacterAnimNotifyState::GetOwnerActor(USkeletalMeshComponent* MeshComp)
{
	// プレイヤーのベースクラス取得
	AActor* pOwner = Cast<AActor>(MeshComp->GetOwner());
	if (!pOwner) { return nullptr; }

	return pOwner;
}