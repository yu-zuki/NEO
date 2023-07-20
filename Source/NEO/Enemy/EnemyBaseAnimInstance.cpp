// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseAnimInstance.h"
#include "EnamyBase.h"

UEnemyBaseAnimInstance::UEnemyBaseAnimInstance()
    : IsIdol(false)
    , IsWalking(false)
    , IsRunning(false)
    , IsAttacking(false)
    , IsReactingToDamage(false)
    , IsDead(false)
{
}

void UEnemyBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    
     auto* Character = Cast<AEnamyBase>(TryGetPawnOwner());
     if (Character)
     {
         IsIdol = Character->IsIdol();
         IsWalking = Character->IsWalking();
         IsRunning = Character->IsRunning();
         IsAttacking = Character->IsAttacking();
         IsReactingToDamage = Character->IsReactingToDamage();
         IsDead = Character->IsDead();
         IsAir = Character->IsAir();
     }
}