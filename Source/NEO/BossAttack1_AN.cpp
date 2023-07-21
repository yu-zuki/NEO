// Fill out your copyright notice in the Description page of Project Settings.
#include "BossAttack1_AN.h"

#include "Animation/AnimNotifies/AnimNotify.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"


#include "OdaBase.h"

void UBossAttack1_AN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// 取得
	TObjectPtr< AOdaBase> aOda = Cast< AOdaBase>(MeshComp->GetOwner());
	//NULLチェック
	if (aOda == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("RemoveOda is not Found"));
		return;
	}
	// 攻撃フラグを切り替える
	aOda->AttackFlagChange();
}
