// Fill out your copyright notice in the Description page of Project Settings.
#include "BossAttack1_AN.h"

#include "Animation/AnimNotifies/AnimNotify.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

#include "OdaBase.h"

//void UBossAttack1_AN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) 
//{
//	// �擾
//	TObjectPtr< AOdaBase> aOda = Cast< AOdaBase>(MeshComp->GetOwner());
//	//NULL�`�F�b�N
//	if (aOda == nullptr)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("RemoveOda is not Found"));
//		return;
//	}
//	// �U���t���O��؂�ւ���
//	aOda->AttackFlagChange();
//}

void UBossAttack1_AN::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	//PlayerGet
	AOdaBase* Oda = Cast<AOdaBase>(MeshComp->GetOwner());
	if (Oda) {
		Oda->CheckOverlap();
	}
}
