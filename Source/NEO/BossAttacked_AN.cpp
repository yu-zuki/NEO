// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAttacked_AN.h"
#include "OdaBase.h"

void UBossAttacked_AN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// �擾
	TObjectPtr< AOdaBase> aOda = Cast< AOdaBase>(MeshComp->GetOwner());
	//NULL�`�F�b�N
	if (aOda == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("RemoveOda is not Found"));
		return;
	}
	// �v���C���[��HP�����炳�Ȃ��悤�ɂ���
	aOda->toPlayerAttacked();
}