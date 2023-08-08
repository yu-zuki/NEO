// Fill out your copyright notice in the Description page of Project Settings.


#include "BossHitEffect_AN.h"
#include "OdaBase.h"

void UBossHitEffect_AN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	// �擾
	TObjectPtr< AOdaBase> aOda = Cast< AOdaBase>(MeshComp->GetOwner());
	//NULL�`�F�b�N
	if (aOda == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("RemoveOda is not Found"));
		return;
	}
	// �Ռ��g������
	aOda->ShockWaveSpawnFlagChange();
}