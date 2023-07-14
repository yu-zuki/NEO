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


protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void EnableTick();

    UPROPERTY(EditDefaultsOnly, Category = "Movement")
        float MovementRadius;
    
    

private:
    UPROPERTY(EditAnywhere, Category = "Rotation")
        bool bIsRotation;
    UPROPERTY(EditAnywhere, Category = "Location")
        bool bIsLocation;
    FTimerHandle TimerHandle;
    FVector InitialLocation;
    FVector StartingLocation;

};