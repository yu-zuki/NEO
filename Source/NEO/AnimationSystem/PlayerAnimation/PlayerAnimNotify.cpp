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
        // �R���{�p���̒ʒm��������
        if (NotifyType == EPlayerNotifyType::NotifyType_SaveCombo)
        {
            pPlayer->ContinuationCombo();
        }
        // �R���{���Z�b�g�̒ʒm��������
        else if (NotifyType == EPlayerNotifyType::NotifyType_ResetCombo)
        {
            pPlayer->ResetCombo();
        }
        // �R���g���[���\�ɂȂ�ʒm��������
        else if (NotifyType == EPlayerNotifyType::NotifyType_CanControl)
        {
            pPlayer->SetControl(true);
        }
        // �A�j���[�V������x������ʒm��������
        else if (NotifyType == EPlayerNotifyType::NotifyType_SlowDown)
        {
            pPlayer->SlowDownDeathAnimationRate();
        }
        // �����蔻��J�n�̏�����������
        else if (NotifyType == EPlayerNotifyType::NotifyType_SetCollision)
        {
            pPlayer->SetCollision();
        }
    }
}

