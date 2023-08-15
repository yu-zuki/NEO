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

	// �x���V�e�B�擾
	UFUNCTION(BlueprintCallable, Category = "Character Parameter")
		FVector GetVelocity() { return Velocity; }

	// �ړ����x�擾
	UFUNCTION(BlueprintCallable, Category = "Character Parameter")
		float GetGroundSpeed() { return GroundSpeed; }

	// �����Ă��邩�擾
	UFUNCTION(BlueprintCallable, Category = "Character Parameter")
		bool  GetShouleMove() { return ShouldMove; }

	// �I�[�i�[���󒆂ɂ��邩�擾
	UFUNCTION(BlueprintCallable, Category = "Character Parameter")
		bool  GetIsFall() { return IsFall; }


private:

	// �I�[�i�[���L�����N�^�[�N���X�Ŏ擾
	ACharacter* OwnerCharacter;

	// �I�[�i�[��MovementComponent�ۑ��p
	class UCharacterMovementComponent* CharacterMovementComp;

	// 
	FVector Velocity;

	//���s���x 
	float GroundSpeed;

	// �����Ă��邩
	bool ShouldMove;

	// �ڒn���ĂȂ���Ԃ��ǂ���. �󒆂ɂ����True 
	bool IsFall;

};
