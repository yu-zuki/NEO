// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Soldier.generated.h"

UCLASS()
class NEO_API ASoldier : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASoldier();

    UPROPERTY(EditAnywhere, Category = "AI")
        float DesiredDistance = 500.0f; // プレイヤーからの望ましい距離（3m）

    UPROPERTY(EditAnywhere, Category = "AI")
        float MovementSpeed = 300.0f; // 移動速度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
        UAnimMontage* Attack;
    UPROPERTY()
        class ACharacter* PlayerCharacter; // プレイヤーキャラクターの参照
protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

private:


    FVector GetPlayerDirection() const;
    float GetDistanceToPlayer() const;

};
