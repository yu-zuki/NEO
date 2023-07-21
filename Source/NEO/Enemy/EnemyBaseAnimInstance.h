// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NEO_API UEnemyBaseAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UEnemyBaseAnimInstance();

    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
        bool IsIdol;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
        bool IsWalking;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
        bool IsRunning;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
        bool IsAttacking;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
        bool IsReactingToDamage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
        bool IsDead;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
        bool IsAir;
};