// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimNotify.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"


/*
 * �֐����@�@�@�@�FNotify()
 * �������e�@�@�@�F�ʒm���������ɍs������
 * �����P�@�@�@�@�FUSkeletalMeshComponent* MeshComp�E�E�E�I�[�i�[�̃��b�V��
 * �����Q�@�@�@�@�FUAnimSequenceBase* Animation�E�E�E�Đ����̃A�j���[�V����
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void UCharacterAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// �I�[�i�[�擾
	AActor* OwnerActor = GetOwnerActor(MeshComp);
	if (!OwnerActor) { return; }


	// �ʒm���������̏���
	NotifyAction(OwnerActor,Animation);
}


/*
 * �֐����@�@�@�@�FGetOwnerActor()
 * �������e�@�@�@�F�I�[�i�[��AActor*�Ŏ擾
 * �����P�@�@�@�@�FUSkeletalMeshComponent* MeshComp�E�E�E�I�[�i�[�̃��b�V��
 * �߂�l�@�@�@�@�F�I�[�i�[��AActor*�Ŏ擾
 */
AActor* UCharacterAnimNotify::GetOwnerActor(USkeletalMeshComponent* MeshComp)
{
	// �v���C���[�̃x�[�X�N���X�擾
	AActor* pOwner = Cast<AActor>(MeshComp->GetOwner());
	if (!pOwner) { return nullptr; }

	return pOwner;
}