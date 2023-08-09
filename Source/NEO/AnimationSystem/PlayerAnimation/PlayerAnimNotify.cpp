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
        // ブランチの名前取得
        FString NotifyName = GetNotifyName_Implementation();

        // コンボ継続の通知が来た時
        if (NotifyName == "SaveCombo")
        {
            pPlayer->ContinuationCombo();
        }
        // コンボリセットの通知が来た時
        else if (NotifyName == "ResetCombo")
        {
            pPlayer->ResetCombo();
        }
        // コントロール可能になる通知が来た時
        else if (NotifyName == "CanControl")
        {
            pPlayer->SetControl(true);
        }
        // アニメーションを遅くする通知が来た時
        else if (NotifyName == "SlowDown")
        {
            pPlayer->SlowDownDeathAnimationRate();
        }
        // 当たり判定開始の処理が来た時
        else if (NotifyName == "SetCollision")
        {
            pPlayer->SetCollision();
        }
    }
}

