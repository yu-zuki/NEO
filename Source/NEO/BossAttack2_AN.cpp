// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAttack2_AN.h"
#include "OdaBase.h"

void UBossAttack2_AN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	// æ¾
	TObjectPtr< AOdaBase> aOda = Cast< AOdaBase>(MeshComp->GetOwner());
	//NULL`FbN
	if (aOda == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("RemoveOda is not Found"));
		return;
	}
	// ÕgðÂ
	aOda->ShockWaveSpawnFlagChange();
}