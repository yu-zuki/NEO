#include "Soldier.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

ASoldier::ASoldier()
{
    PrimaryActorTick.bCanEverTick = true;

    // Initialize variables
    bIsCharging = false;
    ChargingTime = 0.0f;
    TimeSinceLastCharge = 0.0f;

    // Initialize movement component
    MovementComponent = GetCharacterMovement();
}

void ASoldier::BeginPlay()
{
    Super::BeginPlay();

    // Find the player character
    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
}

void ASoldier::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Calculate the distance to the player
    float DistanceToPlayer = FVector::Dist(PlayerCharacter->GetActorLocation(), GetActorLocation());

    // Check if the player is in detection range
    if (DistanceToPlayer <= DetectionRange)
    {
        // If the soldier is not currently charging, start the charge
        if (!bIsCharging)
        {
            StartCharge();
        }

        // If the soldier is charging, move towards the player
        if (bIsCharging)
        {
            MoveTowardsPlayer(DeltaTime);
        }
    }
    else
    {
        // If the player is out of range, stop the charge
        if (bIsCharging)
        {
            StopCharge();
        }
    }
}

void ASoldier::MoveTowardsPlayer(float DeltaTime)
{
    // Calculate the direction to the player
    FVector DirectionToPlayer = (PlayerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();

    // Move the soldier towards the player
    AddMovementInput(DirectionToPlayer, DeltaTime);
}

void ASoldier::StartCharge()
{
    bIsCharging = true;
    bIsRotation = true;
    MovementComponent->MaxWalkSpeed = ChargeSpeed;
}

void ASoldier::StopCharge()
{
    bIsCharging = false;
    bIsRotation = false;
    MovementComponent->MaxWalkSpeed = NormalSpeed;
}

void ASoldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}




