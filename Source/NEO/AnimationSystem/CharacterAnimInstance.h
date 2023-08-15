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

	// 開始時処理
	virtual void NativeBeginPlay() override;

	// 毎フレーム処理(Tick) 
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


protected:

	// ベロシティ取得
	UFUNCTION(BlueprintCallable, Category = "Character Parameter")
		FVector GetVelocity() { return Velocity; }

	// 移動速度取得
	UFUNCTION(BlueprintCallable, Category = "Character Parameter")
		float GetGroundSpeed() { return GroundSpeed; }

	// 動いているか取得
	UFUNCTION(BlueprintCallable, Category = "Character Parameter")
		bool  GetShouleMove() { return ShouldMove; }

	// オーナーが空中にいるか取得
	UFUNCTION(BlueprintCallable, Category = "Character Parameter")
		bool  GetIsFall() { return IsFall; }


private:

	// オーナーをキャラクタークラスで取得
	ACharacter* OwnerCharacter;

	// オーナーのMovementComponent保存用
	class UCharacterMovementComponent* CharacterMovementComp;

	// 
	FVector Velocity;

	//歩行速度 
	float GroundSpeed;

	// 動いているか
	bool ShouldMove;

	// 接地してない状態かどうか. 空中にいるとTrue 
	bool IsFall;

};
