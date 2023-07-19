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
        float MovementSpeed = 100.0f; // à⁄ìÆë¨ìx
    UPROPERTY(EditAnywhere, Category = "AI")
        float DesiredDistance = -500.0f;//ñ]Ç‹ÇµÇ¢ãóó£
protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;
    FVector GetPlayerDirection() const;
    float GetDistanceToPlayer() const;
public:
    UPROPERTY(EditAnywhere, Category = "Rotation")
        bool bIsRotation;
    //ÇRïbÇ≤Ç∆Ç…ìÆÇ≠É^ÉCÉ}Å[
    FTimerHandle MoveStraightTimerHandle;

    
};