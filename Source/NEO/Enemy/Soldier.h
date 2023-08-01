#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnamyBase.h"
#include "Soldier.generated.h"

UCLASS()
class NEO_API ASoldier : public AEnamyBase
{
    GENERATED_BODY()

public:
    ASoldier();
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    // Determines if the soldier is currently charging
    bool bIsCharging;

    // Time since the soldier started charging
    float ChargingTime;

    // Time since the soldier last charged
    float TimeSinceLastCharge;

    // Charging duration
    const float ChargeDuration = 2.0f;

    // Time between charges
    const float TimeBetweenCharges = 3.0f;

    // Soldier movement component
    UCharacterMovementComponent* MovementComponent;

    // The player character to attack
    ACharacter* PlayerCharacter;

    // Distance at which the soldier will start charging
    const float DetectionRange = 500.0f;

    // Move the soldier towards the player
    void MoveTowardsPlayer(float DeltaTime);

    // Start the soldier's charge attack
    void StartCharge();

    // Stop the soldier's charge attack
    void StopCharge();
    float NormalSpeed = 600.0f;

    // Speed during a charge
    float ChargeSpeed = 1200.0f;
};

    