// Fill out your copyright notice in the Description page of Project Settings.


#include "GunMan.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerSystem\PlayerCharacter.h"
#include "Camera/PlayerCameraManager.h"
#include "Engine/World.h"
#include "Bullet.h"
#include "TrajectoryBullet.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGunMan::AGunMan()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    // キャラクターの移動方式を設定
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
    MaxHealth = 100;
    Health = MaxHealth;
    
   
}

// Called when the game starts or when spawned
void AGunMan::BeginPlay()
{
	Super::BeginPlay();
    // プレイヤーキャラクターの参照を取得
    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    
    GetWorldTimerManager().SetTimer(TimerHandle_Blink, this, &AGunMan::BlinkTrajectoryBullet, 2.0f, false);
}
    


// Called every frame
void AGunMan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Health = MaxHealth;
    FacePlayer();
    if (PlayerCharacter)
    {
        // プレイヤーとの距離を取得
        float DistanceToPlayer = GetDistanceToPlayer();

        // プレイヤーとの距離が望ましい距離よりも離れている場合、プレイヤーに近づく
        if (DistanceToPlayer > DesiredDistance)
        {
            FVector PlayerDirection = GetPlayerDirection();
            AddMovementInput(PlayerDirection);
            //プレイヤーのほうを向く
            FRotator TargetRotation = PlayerDirection.Rotation();
            SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 5.0f));
        }
        else if (DistanceToPlayer < DesiredDistance - 100.0f) // プレイヤーが望ましい距離-100以下に入った場合
        {
            // プレイヤーから離れる
            FVector PlayerDirection = GetPlayerDirection();
            AddMovementInput(-PlayerDirection);
        }
    }
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
void AGunMan::SpawnBullet()
{
    FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f; // 100.0fは適宜調整してください
    FRotator SpawnRotation = GetActorRotation();
    GetWorld()->SpawnActor<ABullet>(BulletClass, SpawnLocation, SpawnRotation);
    
    
}

void AGunMan::BlinkTrajectoryBullet()
{
   
    FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 200.0f; // 200.0fは適宜調整してください
    FRotator SpawnRotation = GetActorRotation();
    ATrajectoryBullet* TrajectoryBullet = GetWorld()->SpawnActor<ATrajectoryBullet>(TrajectoryBulletClass, SpawnLocation, SpawnRotation);

    if (TrajectoryBullet)
    {
       
        TrajectoryBullet->StartBlinking();

        
        FTimerHandle TimerHandle_BulletSpawn;
        GetWorldTimerManager().SetTimer(TimerHandle_BulletSpawn, this, &AGunMan::SpawnBullet, 2.0f, false);
    }

    
    FTimerHandle TimerHandle_NextSequence;
    GetWorldTimerManager().SetTimer(TimerHandle_NextSequence, this, &AGunMan::BlinkTrajectoryBullet, 4.0f, false);
}

FVector AGunMan::GetPlayerDirection() const
{
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector GunManLocation = GetActorLocation();
    return FVector(PlayerLocation.X - GunManLocation.X, PlayerLocation.Y - GunManLocation.Y, 0.0f).GetSafeNormal();
}

float AGunMan::GetDistanceToPlayer() const
{
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector GunManLocation = GetActorLocation();
    return FVector::Distance(PlayerLocation, GunManLocation);
}

void AGunMan::FacePlayer()
{
    // プレイヤーキャラクターの位置を取得
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();

    // プレイヤーキャラクターの位置に向かってLancerを回転させる
    FRotator TargetRotation = (PlayerLocation - GetActorLocation()).Rotation();
    SetActorRotation(TargetRotation);
}
/*
void AGunMan::ApplyDamage(float DamageAmount, float DeltaTime)
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
*/
