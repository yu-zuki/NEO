// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimNotify.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "NEO/Enemy/EnamyBase.h"

void UEnemyAnimNotify::NotifyAction(AActor* _Owner)
{
    AEnamyBase* pEnemy = Cast<AEnamyBase>(_Owner);


    if (pEnemy)
    {
        // �u�����`�̖��O�擾
        FString NotifyName = GetNotifyName_Implementation();

        if (NotifyName == "EnemyCollisionOn")
        {
            pEnemy->CheckCollisonOn();
        }
        else if (NotifyName == "EnemyCollisionOff")
        {
            pEnemy->CheckCollisonOff();
        }


    }
}

