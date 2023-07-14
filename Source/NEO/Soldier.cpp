#include "Soldier.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

ASoldier::ASoldier()
{
    // キャラクターの移動方式を設
    MaxHealth = 100;
    Health = MaxHealth;
    bIsJumping = false;
    // キャラクターの移動方式を設定
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

    
    PrimaryActorTick.bCanEverTick = true;
    TimeToStopMoving = -1.0f;
 
}



void ASoldier::BeginPlay()
{
    Super::BeginPlay();

    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    // Start moving towards the player
    TimeStartedMoving = GetWorld()->GetTimeSeconds();
    TimeToStopMoving = TimeStartedMoving + 5.0f;
    GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}


void ASoldier::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // キャラクターの位置を取得
    FVector CharacterLocation = GetActorLocation();

    // 自分の座標を取得
    FVector MyLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

    // キャラクターの位置と自分の位置を比較してY軸より前にいるかどうかを判定
    bIsRotation = CharacterLocation.Y > MyLocation.Y;
    // bIsRotationがtrueなら
    if (bIsRotation)
    {
        FRotator NewRotation = GetActorRotation();
        NewRotation.Yaw = -90.0f;
        SetActorRotation(NewRotation);
        
    }
    else
    {
        FRotator NewRotation = GetActorRotation();
        NewRotation.Yaw = 90.0f;
        SetActorRotation(NewRotation);
    }

    float CurrentTime = GetWorld()->GetTimeSeconds();
    // If the soldier should be moving towards the player
    if (TimeToStopMoving >= 0.0f && CurrentTime < TimeToStopMoving)
    {
        // Calculate the direction to the player
        FVector DirectionToPlayer = PlayerCharacter->GetActorLocation() - GetActorLocation();
        DirectionToPlayer.Normalize();

        // Move towards the player
        AddMovementInput(DirectionToPlayer);
    }
    // If the soldier should be resting
    else if (TimeToStopMoving >= 0.0f && CurrentTime < TimeToStartMovingAgain)
    {
        // Do nothing (rest)
    }
    // If the soldier should start moving towards the player again
    else if (TimeToStartMovingAgain >= 0.0f && CurrentTime >= TimeToStartMovingAgain)
    {
        // Start moving towards the player again
        TimeStartedMoving = CurrentTime;
        TimeToStopMoving = TimeStartedMoving + 2.0f;
        TimeToStartMovingAgain = TimeToStopMoving + 3.0f; // Start moving again after 3 seconds of rest
    }
}









