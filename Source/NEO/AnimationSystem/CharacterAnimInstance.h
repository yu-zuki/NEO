// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

UCLASS()
class NEO_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


public:

	// �J�n������
	virtual void NativeBeginPlay() override;

	// ���t���[������(Tick) 
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;




protected:

	// �I�[�i�[���L�����N�^�[�N���X�Ŏ擾
	ACharacter* OwnerCharacter;

	// �I�[�i�[��MovementComponent�ۑ��p
	class UCharacterMovementComponent* CharacterMovementComp;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Owner Status")
		FVector Velocity;

	//���s���x
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Owner Status")
		float GroundSpeed;

	// �����Ă��邩
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Owner Status")
		bool ShouldMove;

	// �󒆂ɂ����Ԃ��ǂ���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Owner Status")
		bool IsFall;
};
