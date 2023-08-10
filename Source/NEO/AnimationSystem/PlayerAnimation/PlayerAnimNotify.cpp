// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimNotify.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "NEO/PlayerSystem/PlayerBase.h"


void UPlayerAnimNotify::NotifyAction(AActor* _Owner)
{
    // プレイヤーのベースクラス取得
    APlayerBase* pPlayer = Cast<APlayerBase>(_Owner);


    if (pPlayer)
    {      
        // コンボ継続の通知が来た時
        if (NotifyType == EPlayerNotifyType::NotifyType_SaveCombo)
        {
            pPlayer->ContinuationCombo();
        }
        // コンボリセットの通知が来た時
        else if (NotifyType == EPlayerNotifyType::NotifyType_ResetCombo)
        {
            pPlayer->ResetCombo();
        }
        // コントロール可能になる通知が来た時
        else if (NotifyType == EPlayerNotifyType::NotifyType_CanControl)
        {
            pPlayer->SetControl(true);
        }
        // アニメーションを遅くする通知が来た時
        else if (NotifyType == EPlayerNotifyType::NotifyType_SlowDown)
        {
            pPlayer->SlowDownDeathAnimationRate();
        }
        // 当たり判定開始の処理が来た時
        else if (NotifyType == EPlayerNotifyType::NotifyType_SetCollision)
        {
            pPlayer->SetCollision();
        }
    }
}

