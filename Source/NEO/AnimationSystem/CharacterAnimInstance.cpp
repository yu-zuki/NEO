// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

void UCharacterAnimInstance::NativeBeginPlay()
{
    // �I�[�i�[���擾����
    OwnerCharacter = Cast<ACharacter>(GetOwningActor());

    // �L�����N�^�[���[�u�����g�擾
    if (OwnerCharacter)
    {
        CharacterMovementComp = OwnerCharacter->GetCharacterMovement();
    }
}


//
void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    // OwnerCharacter�̌��݂̃X�e�[�^�X�擾
    if (OwnerCharacter)
    {
        // ���x���擾����
        Velocity = OwnerCharacter->GetVelocity();

        // �ړ��X�s�[�h�擾
        GroundSpeed = Velocity.Size();

        // 
        ShouldMove = (CharacterMovementComp->GetCurrentAcceleration() != FVector::ZeroVector) && (GroundSpeed > 3.f);
        
        // �󒆂ɂ��邩�ǂ������擾����
        IsFall = CharacterMovementComp->IsFalling();
    }
}

