#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnamyBase.h"
#include "Soldier.generated.h"

UCLASS()
class NEO_API ASoldier : public AEnamyBase
{
    GENERATED_BODY()

public:
    ASoldier();
    UPROPERTY(EditAnywhere, Category = "AI")
        float MovementSpeed = 100.0f; // 移動速度
    UPROPERTY(EditAnywhere, Category = "AI")
        float DesiredDistance = -500.0f;//望ましい距離
protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;
    FVector GetPlayerDirection() const;
    float GetDistanceToPlayer() const;
public:
        bool bIsRotation;
    //３秒ごとに動くタイマー
    FTimerHandle MoveStraightTimerHandle;

    
};