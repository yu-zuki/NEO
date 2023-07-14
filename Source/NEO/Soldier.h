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

  

  

private:
    UPROPERTY(EditAnywhere, Category = "Rotation")
        bool bIsRotation;
    UPROPERTY(EditAnywhere, Category = "Location")
        bool bIsLocation;
    // The time the soldier started moving towards the player
   

    float TimeStartedMoving;

    // The time the soldier should stop moving towards the player
    float TimeToStopMoving;

    // The time the soldier should start moving towards the player again
    float TimeToStartMovingAgain;

    // The player character
    ACharacter* PlayerCharacter;

};