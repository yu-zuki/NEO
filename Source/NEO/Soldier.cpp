#include "Soldier.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "AIController.h"
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
    //攻撃の間隔
    DelayBeforeAttack = 2.0f;
    AttackInterval = 5.0f;
    MoveSpeed = 600.0f;
}



void ASoldier::BeginPlay()
{
    Super::BeginPlay();

    // プレイヤーキャラクターの参照を取得
    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    // 2秒後に攻撃を開始する
    GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ASoldier::StartAttack, DelayBeforeAttack, false);
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

}
void ASoldier::StartAttack()
{
    // プレイヤーに向かって突撃する
    MoveToPlayer();

    // 指定の間隔で攻撃を繰り返す
    GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ASoldier::RepeatAttack, AttackInterval, true);
}
void ASoldier::RepeatAttack()
{
    /*
     // プレイヤーとの距離を計算
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    if (PlayerPawn)
    {
        FVector PlayerLocation = PlayerPawn->GetActorLocation();
        FVector SoldierLocation = GetActorLocation();
        float Distance = FVector::Dist(PlayerLocation, SoldierLocation);

        // プレイヤーとの距離が一定範囲内でなければプレイヤーに向かって突撃する
        if (Distance > MaxDistanceFromPlayer)
        {
            MoveToPlayer();
        }
    
     }
    */
   
    MoveToPlayer();
}

void ASoldier::MoveToPlayer()
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    if (PlayerPawn)
    {
        PlayerLocation = PlayerPawn->GetActorLocation();

        // プレイヤーの方向に進む
        FVector Direction = PlayerLocation - GetActorLocation();
        Direction.Z = 0.0f;
        Direction.Normalize();

        // 移動速度を設定
        UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
        if (MovementComponent)
        {
            MovementComponent->MaxWalkSpeed = MoveSpeed;
        }

        // まっすぐ進む
        AddMovementInput(Direction, 1.0f);
    }
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





