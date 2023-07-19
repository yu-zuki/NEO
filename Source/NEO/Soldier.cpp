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
    // キャラクターの移動方式を設定
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
    Health = MaxHealth;
    bIsJumping = false;
}



void ASoldier::BeginPlay()
{
    Super::BeginPlay();

    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

}


void ASoldier::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // キャラクターの位置を取得
    FVector CharacterLocation = GetActorLocation();

    // 自分の座標を取得
    FVector MyLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
    // プレイヤーとの距離を取得
    float DistanceToPlayer = GetDistanceToPlayer();

    // プレイヤーとの距離が望ましい距離よりも離れている場合、プレイヤーに近づく
    if (DistanceToPlayer > DesiredDistance)
    {
        FVector PlayerDirection = GetPlayerDirection();
        AddMovementInput(PlayerDirection);
        MovementSpeed = 100.0f;
        bIsRotation = true;
    }
    else if (DistanceToPlayer < DesiredDistance +100.0f) // プレイヤーが望ましい距離-100以下に入った場合
    {
        
        MovementSpeed = 200.0f;
        
        bIsRotation = false;
    }
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

}

FVector ASoldier::GetPlayerDirection() const
{
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector GunManLocation = GetActorLocation();
    return FVector(PlayerLocation.X - GunManLocation.X, PlayerLocation.Y - GunManLocation.Y, 0.0f).GetSafeNormal();
}

float ASoldier::GetDistanceToPlayer() const
{
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector GunManLocation = GetActorLocation();
    return FVector::Distance(PlayerLocation, GunManLocation);
}






