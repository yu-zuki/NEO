// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimNotify.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "NEO/PlayerSystem/PlayerBase.h"

void UPlayerAnimNotify::NotifyAction(AActor* _Owner)
{
    // �v���C���[�̃x�[�X�N���X�擾
    APlayerBase* pPlayer = Cast<APlayerBase>(_Owner);


    if (pPlayer)
    {
        // �u�����`�̖��O�擾
        FString NotifyName = GetNotifyName_Implementation();

        if (NotifyName == "SaveCombo")
        {
            pPlayer->ContinuationCombo();
        }
        else if (NotifyName == "ResetCombo")
        {
            pPlayer->ResetCombo();
        }
        else if (NotifyName == "CanControl")
        {
            pPlayer->SetControl(true);
        }
        else if (NotifyName == "SlowDown")
        {
            pPlayer->SlowDownDeathAnimationRate();
        }
        else if (NotifyName == "Collision")
        {
            pPlayer->SetCollision();
        }
    }
}

