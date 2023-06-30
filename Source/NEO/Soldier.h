#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Soldier.generated.h"

UCLASS()
class NEO_API ASoldier : public AEnamyBase
{
    GENERATED_BODY()

public:
    ASoldier();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Movement")
        float Acceleration; // 加速度

    UPROPERTY(EditAnywhere, Category = "Movement")
        float MaxSpeed; // 最大速度

    UPROPERTY(EditAnywhere, Category = "Movement")
        float DetectionRange; // 検知範囲

    ACharacter* PlayerCharacter; // プレイヤーキャラクターへの参照
    FVector TargetLocation; // 目標位置
    bool bMovingForward; // 前進中フラグ

    float TimeSinceLastMovement; // 前回の動作からの経過時間

    void MoveForward(float Value);
    void DetectTarget();
    void SetNewTargetLocation();
    bool IsPlayerInDetectionRange();
    void StopMovement();
    void RotateTowardsPlayer();
};