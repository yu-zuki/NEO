// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimNotify.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "NEO/Enemy/EnamyBase.h"
#include "NEO/Enemy/GunMan.h"
#include "NEO/Enemy/Pusher.h"

void UEnemyAnimNotify::NotifyAction(AActor* _Owner, UAnimSequenceBase* Animation)
{
    AEnamyBase* pEnemy = Cast<AEnamyBase>(_Owner);

    AGunMan* pGunMan = Cast<AGunMan>(_Owner);

    APusher* pPusher = Cast<APusher>(_Owner);

    if (pEnemy)
    {
        if (NotifyType == EEnemyNotifyType::NotifyType1)
        {
            pEnemy->CheckCollisonOn();
        }
        else if (NotifyType == EEnemyNotifyType::NotifyType2)
        {
            pEnemy->CheckCollisonOff();
        }

        else if (NotifyType==EEnemyNotifyType::NotifyType3)
        {
            pGunMan->SpawnTrajectoryBullet();
        }
        else if (NotifyType == EEnemyNotifyType::NotifyType4)
        {
            pGunMan->ReplaceWithBullet();
        }
        else if (NotifyType == EEnemyNotifyType::NotifyType5)
        {
            pPusher->SpawnRolling();
        }
        else if (NotifyType == EEnemyNotifyType::NotifyType6)
        {
            pPusher->DetachRolling();
        
        }
    }
}

