
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimNotifyState.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "NEO/PlayerSystem/PlayerBase.h"


/*
 * �֐����@�@�@�@�FNotifyAction()
 * �������e�@�@�@�F�ʒm���ɍs������
 * �����P�@�@�@�@�FAActor* _Owner�E�E�E�I�[�i�[
 * �����Q�@�@�@�@�FUAnimSequenceBase* Animation�E�E�E�Đ�����A�j���[�V����
 * �����R�@�@�@�@�Ffloat TotalDuration�E�E�E�Đ��ɂ����鎞��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UPlayerAnimNotifyState::NotifyBeginAction(AActor* _Owner, UAnimSequenceBase* Animation, float TotalDuration)
{
    APlayerBase* pPlayer = Cast<APlayerBase>(_Owner);


    if (pPlayer)
    {
        // �A�j���[�V�����ł̈ړ��J�n
        if (NotifyType == EPlayerNotifyStateType::NotifyType_RootMotion_Attack1)
        {
            pPlayer->SetEnableRootMotion(true,2.f);
        }
        else if (NotifyType == EPlayerNotifyStateType::NotifyType_RootMotion_Attack2)
        {
            pPlayer->SetEnableRootMotion(true,5.f);
        }
        else if (NotifyType == EPlayerNotifyStateType::NotifyType_RootMotion_Attack3)
        {
            pPlayer->SetEnableRootMotion(true,8.f);
        }
        else if (NotifyType == EPlayerNotifyStateType::NotifyType_RootMotion_KnockBack)
        {
            pPlayer->SetEnableRootMotion(true, -4.f);
        }

    }
}


/*
 * �֐����@�@�@�@�FNotifyAction()
 * �������e�@�@�@�F�ʒm���ɍs������
 * �����P�@�@�@�@�FAActor* _Owner�E�E�E�I�[�i�[
 * �����Q�@�@�@�@�FUAnimSequenceBase* Animation�E�E�E�Đ�����A�j���[�V����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UPlayerAnimNotifyState::NotifyEndAction(AActor* _Owner, UAnimSequenceBase* Animation)
{
    APlayerBase* pPlayer = Cast<APlayerBase>(_Owner);


    if (pPlayer)
    {
        // �A�j���[�V�����ł̈ړ��I��
        if (NotifyType == EPlayerNotifyStateType::NotifyType_RootMotion_Attack1)
        {
            pPlayer->SetEnableRootMotion(false);
        }
        else if (NotifyType == EPlayerNotifyStateType::NotifyType_RootMotion_Attack2)
        {
            pPlayer->SetEnableRootMotion(false);
        }
        else if (NotifyType == EPlayerNotifyStateType::NotifyType_RootMotion_Attack3)
        {
            pPlayer->SetEnableRootMotion(false);
        }
        else if (NotifyType == EPlayerNotifyStateType::NotifyType_RootMotion_KnockBack)
        {
            pPlayer->SetEnableRootMotion(false);
        }
    }
}


