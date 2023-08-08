// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NEO_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


protected:
	UPROPERTY(BlueprintReadWrite)
		FVector Velocity;

	//���s���x 
	UPROPERTY(BlueprintReadWrite)
		float GroundSpeed;

	// �����Ă��邩
	UPROPERTY(BlueprintReadWrite)
		bool ShouldMove;

	//�ڒn���ĂȂ���Ԃ��ǂ���. �󒆂ɂ����True 
	UPROPERTY(BlueprintReadWrite)
		bool IsFall;

protected:
	// �x���V�e�B�擾
	UFUNCTION(BlueprintCallable, Category = "Character Parameter")
		FVector GetVelocity() { return Velocity; }

	// �ړ����x�擾
	UFUNCTION(BlueprintCallable, Category = "Character Parameter")
		float GetGroundSpeed() { return GroundSpeed; }

public:
	// �J�n������
	virtual void NativeBeginPlay() override;

	// ���t���[������(Tick) 
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
private:
	ACharacter* OwnerCharacter;
	class UCharacterMovementComponent* CharacterMovementComp;
};
