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
    
    //重力加速度
    fGravityAcceleration = 9.8f;
    //ジャンプ時間
    fJumpTime = 0.0f;
    //ジャンプ高さ
    fJumpHeight = 100.0f;
    //ジャンプ開始時の位置
    vJumpStartLocation = GetActorLocation();
    
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
void ASoldier::ApplyDamage(float DamageAmount, float DeltaTime)
{
    Health -= DamageAmount;

    if (Health <= 0.0f)
    {
        //ジャンプ処理
        JumpingByGravity(DeltaTime);
        PlayAnimMontage(Death, 1, NAME_None);

    }
    else
    {
        PlayAnimMontage(Damage_Reaction, 1, NAME_None);
    }
}
void ASoldier::StartJumpByGravity(float JumpHeight, float GravityAcceleration)
{
    if (!bIsJumping)
    {
        //ジャンプを開始
        fJumpTime = 0.0f;
        fJumpHeight = JumpHeight;
        fGravityAcceleration = GravityAcceleration;

        bIsJumping = true;
    }
}
void ASoldier::JumpingByGravity(float DeltaTime)
{
    if (bIsJumping)
    {
        //ジャンプ中
        fJumpTime += DeltaTime;

        //ジャンプの移動距離を計算
        float fCurrentJumpHeight = fJumpHeight * FMath::Sin(fJumpTime) - 0.5f * fGravityAcceleration * FMath::Pow(fJumpTime, 2.0f);

        //キャラクターに移動距離を反映
        FVector vCurrentLocation = GetActorLocation();
        vCurrentLocation.Z = vJumpStartLocation.Z + fCurrentJumpHeight;
        SetActorLocation(vCurrentLocation);

        //ジャンプを終了するかどうかを判断
        if (fCurrentJumpHeight <= 0.0f)
        {
            EndJumpByGravity();
        }

    }
}

/**
 * 重力ジャンプを終了する
 */
void ASoldier::EndJumpByGravity()
{
    //ジャンプを終了
    bIsJumping = false;

    //ジャンプ時間をリセット
    fJumpTime = 0.0f;

    //ジャンプ高さをリセット
    fJumpHeight = 0.0f;

    //重力加速度をリセット
    fGravityAcceleration = 0.0f;
}
// Called to bind functionality to input
void ASoldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}






