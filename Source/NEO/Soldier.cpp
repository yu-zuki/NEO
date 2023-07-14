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
    MovementRadius = 500.0f; // 移動範囲の半径を設定
    bIsLocation = true;
}



void ASoldier::BeginPlay()
{
    Super::BeginPlay();

    InitialLocation = GetActorLocation(); // 初期位置を保存

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
    // ランダムな移動ベクトルを生成
    FVector RandomMovement = FVector(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f), 0.0f);
    RandomMovement.Normalize();

    // 移動範囲内のランダムな位置を計算
    FVector TargetLocation = InitialLocation + RandomMovement * MovementRadius;

    // プレイヤーとの距離が一定範囲内であれば移動する
    FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
    float DistanceToPlayer = FVector::Dist(InitialLocation, PlayerLocation);
    bIsRotation = CharacterLocation.Y > MyLocation.Y;
   
     if (DistanceToPlayer > MovementRadius)
        {
        // 移動先に向かって移動する
        FVector MovementDirection = TargetLocation - GetActorLocation();
        MovementDirection.Normalize();
        SetActorLocation(GetActorLocation() + MovementDirection * DeltaTime * GetCharacterMovement()->MaxWalkSpeed);
        
       
        }
    
   
    else
    {
        FVector ForwardDirection = GetActorForwardVector();
        SetActorLocation(GetActorLocation() + ForwardDirection *10.0f);
        
    }

}
void ASoldier::EnableTick()
{
    SetActorTickEnabled(true);
}
// Called to bind functionality to input
void ASoldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}






