// Fill out your copyright notice in the Description page of Project Settings.


#include "GunMan.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NEO/PlayerSystem\PlayerCharacter.h"
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
	
    
        if (PlayerCharacter)
        {
            // プレイヤーとの距離を取得
            float DistanceToPlayer = GetDistanceToPlayer();

            // プレイヤーとの距離が望ましい距離よりも離れている場合、プレイヤーに近づく
            if (DistanceToPlayer > DesiredDistance)
            {
                FVector PlayerDirection = GetPlayerDirection();
                AddMovementInput(PlayerDirection);

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

    
}
void AGunMan::SpawnBullet()
{
   
        FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f; // 100.0fは適宜調整してください
        FRotator SpawnRotation = GetActorRotation();
        GetWorld()->SpawnActor<ABullet>(BulletClass, SpawnLocation, SpawnRotation);
        MovementSpeed = 200.0f;

    
    
    
}

void AGunMan::BlinkTrajectoryBullet()
{
        FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 50.0f;
        FRotator SpawnRotation = GetActorRotation();
        ATrajectoryBullet* TrajectoryBullet = GetWorld()->SpawnActor<ATrajectoryBullet>(TrajectoryBulletClass, SpawnLocation, SpawnRotation);

        if (TrajectoryBullet)
        {

            TrajectoryBullet->StartBlinking();


            FTimerHandle TimerHandle_BulletSpawn;
            GetWorldTimerManager().SetTimer(TimerHandle_BulletSpawn, this, &AGunMan::SpawnBullet, 2.0f, false);
            MovementSpeed = 0.0f;
        }


        FTimerHandle TimerHandle_NextSequence;
        GetWorldTimerManager().SetTimer(TimerHandle_NextSequence, this, &AGunMan::BlinkTrajectoryBullet, 4.0f, false);
        MovementSpeed = 200.0f;
    
  
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
