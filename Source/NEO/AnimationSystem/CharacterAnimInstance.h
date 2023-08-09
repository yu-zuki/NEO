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

	//歩行速度 
	UPROPERTY(BlueprintReadWrite)
		float GroundSpeed;

	// 動いているか
	UPROPERTY(BlueprintReadWrite)
		bool ShouldMove;

	//接地してない状態かどうか. 空中にいるとTrue 
	UPROPERTY(BlueprintReadWrite)
		bool IsFall;

protected:
	// ベロシティ取得
	UFUNCTION(BlueprintCallable, Category = "Character Parameter")
		FVector GetVelocity() { return Velocity; }

	// 移動速度取得
	UFUNCTION(BlueprintCallable, Category = "Character Parameter")
		float GetGroundSpeed() { return GroundSpeed; }

public:
	// 開始時処理
	virtual void NativeBeginPlay() override;

	// 毎フレーム処理(Tick) 
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
private:
	ACharacter* OwnerCharacter;
	class UCharacterMovementComponent* CharacterMovementComp;
};
