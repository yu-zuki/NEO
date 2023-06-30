// Fill out your copyright notice in the Description page of Project Settings.
#include "Soldier.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"

ASoldier::ASoldier()
{
    PrimaryActorTick.bCanEverTick = true;

    // 初期設定値
    Acceleration = 500.0f;
    MaxSpeed = 1000.0f;
    DetectionRange = 500.0f;
    bMovingForward = false;
    TimeSinceLastMovement = 0.0f;
   
}

void ASoldier::BeginPlay()
{
    Super::BeginPlay();
    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    SetNewTargetLocation();
}

void ASoldier::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TimeSinceLastMovement += DeltaTime;
    DetectTarget();

    if (bMovingForward)
    {
        MoveForward(1.0f);

        // 一定距離通過後、停止して新たな目標位置を設定
        if (GetActorLocation().X >= TargetLocation.X)
        {
            bMovingForward = false;
            SetNewTargetLocation();
        }
    }
}


void ASoldier::MoveForward(float Value)
{
    if (Value != 0.0f)
    {
        // 加速度を適用して速度を変更
        FVector Direction = GetActorForwardVector();
        FVector Velocity = Direction * Acceleration * Value;
        FVector NewLocation = GetActorLocation() + Velocity * GetWorld()->DeltaTimeSeconds;

        // 速度制限
        float CurrentSpeed = GetVelocity().Size();
        if (CurrentSpeed + Acceleration * GetWorld()->DeltaTimeSeconds <= MaxSpeed)
        {
            SetActorLocation(NewLocation);
        }
    }
}

void ASoldier::DetectTarget()
{
     if (PlayerCharacter)
    {
        // プレイヤーとの距離を測定
        float Distance = FVector::Distance(GetActorLocation(), PlayerCharacter->GetActorLocation());

        // 一定範囲に近づいたら移動を開始
        if (!bMovingForward && Distance <= DetectionRange && TimeSinceLastMovement >= 2.0f)
        {
            bMovingForward = true;
            TimeSinceLastMovement = 0.0f; // 再度同じことをするために経過時間をリセット
        }
    }
}
void ASoldier::SetNewTargetLocation()
{
    if (PlayerCharacter)
    {
        TargetLocation = PlayerCharacter->GetActorLocation();
    }
}
bool ASoldier::IsPlayerInDetectionRange()
{
    if (PlayerCharacter && !PlayerCharacter->IsPendingKill())
    {
        float Distance = FVector::Distance(GetActorLocation(), PlayerCharacter->GetActorLocation());
        return Distance <= DetectionRange;
    }
    return false;
}

void ASoldier::StopMovement()
{
    bMovingForward = false;
    SetActorRotation((PlayerCharacter->GetActorLocation() - GetActorLocation()).Rotation());
}
void ASoldier::RotateTowardsPlayer()
{
    if (PlayerCharacter && !PlayerCharacter->IsPendingKill())
    {
        FVector TargetDirection = PlayerCharacter->GetActorLocation() - GetActorLocation();
        TargetDirection.Z = 0.0f; // Yawのみに制限するためにZ軸を0に設定
        FRotator TargetRotation = TargetDirection.Rotation();

        // プレイヤーの方向を向く
        SetActorRotation(TargetRotation);
    }
}
