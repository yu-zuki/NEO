// Fill out your copyright notice in the Description page of Project Settings.
#include "Soldier.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"

ASoldier::ASoldier()
{
    PrimaryActorTick.bCanEverTick = true;

    // �����ݒ�l
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

        // ��苗���ʉߌ�A��~���ĐV���ȖڕW�ʒu��ݒ�
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
        // �����x��K�p���đ��x��ύX
        FVector Direction = GetActorForwardVector();
        FVector Velocity = Direction * Acceleration * Value;
        FVector NewLocation = GetActorLocation() + Velocity * GetWorld()->DeltaTimeSeconds;

        // ���x����
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
        // �v���C���[�Ƃ̋����𑪒�
        float Distance = FVector::Distance(GetActorLocation(), PlayerCharacter->GetActorLocation());

        // ���͈͂ɋ߂Â�����ړ����J�n
        if (!bMovingForward && Distance <= DetectionRange && TimeSinceLastMovement >= 2.0f)
        {
            bMovingForward = true;
            TimeSinceLastMovement = 0.0f; // �ēx�������Ƃ����邽�߂Ɍo�ߎ��Ԃ����Z�b�g
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
        TargetDirection.Z = 0.0f; // Yaw�݂̂ɐ������邽�߂�Z����0�ɐݒ�
        FRotator TargetRotation = TargetDirection.Rotation();

        // �v���C���[�̕���������
        SetActorRotation(TargetRotation);
    }
}
