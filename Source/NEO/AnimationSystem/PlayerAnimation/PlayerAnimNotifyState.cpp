
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimNotifyState.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "NEO/PlayerSystem/PlayerBase.h"


void UPlayerAnimNotifyState::NotifyBeginAction(AActor* _Owner, UAnimSequenceBase* Animation, float TotalDuration)
{
    APlayerBase* pPlayer = Cast<APlayerBase>(_Owner);


    if (pPlayer)
    {
        if (NotifyType == EPlayerNotifyStateType::NotifyType_Combo)
        {
            pPlayer->ContinuationCombo();
        }
    }
}

void UPlayerAnimNotifyState::NotifyEndAction(AActor* _Owner, UAnimSequenceBase* Animation)
{
    APlayerBase* pPlayer = Cast<APlayerBase>(_Owner);


    if (pPlayer)
    {
        if (NotifyType == EPlayerNotifyStateType::NotifyType_Combo)
        {
            pPlayer->ResetCombo();
        }
    }
}


