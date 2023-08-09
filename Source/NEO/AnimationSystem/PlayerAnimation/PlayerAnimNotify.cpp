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

        // �R���{�p���̒ʒm��������
        if (NotifyName == "SaveCombo")
        {
            pPlayer->ContinuationCombo();
        }
        // �R���{���Z�b�g�̒ʒm��������
        else if (NotifyName == "ResetCombo")
        {
            pPlayer->ResetCombo();
        }
        // �R���g���[���\�ɂȂ�ʒm��������
        else if (NotifyName == "CanControl")
        {
            pPlayer->SetControl(true);
        }
        // �A�j���[�V������x������ʒm��������
        else if (NotifyName == "SlowDown")
        {
            pPlayer->SlowDownDeathAnimationRate();
        }
        // �����蔻��J�n�̏�����������
        else if (NotifyName == "SetCollision")
        {
            pPlayer->SetCollision();
        }
    }
}

