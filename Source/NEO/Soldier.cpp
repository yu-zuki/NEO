#include "Soldier.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

ASoldier::ASoldier()
{
    // �L�����N�^�[�̈ړ��������
    MaxHealth = 100;
    Health = MaxHealth;
    bIsJumping = false;
    // �L�����N�^�[�̈ړ�������ݒ�
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
    // �L�����N�^�[�̈ʒu���擾
    FVector CharacterLocation = GetActorLocation();

    // �����̍��W���擾
    FVector MyLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

    // �L�����N�^�[�̈ʒu�Ǝ����̈ʒu���r����Y�����O�ɂ��邩�ǂ����𔻒�
    bIsRotation = CharacterLocation.Y > MyLocation.Y;
    // bIsRotation��true�Ȃ�
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









