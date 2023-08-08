// Fill out your copyright notice in the Description page of Project Settings.


#include "GunMan.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NEO/PlayerSystem\PlayerCharacter.h"
#include "Camera/PlayerCameraManager.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Bullet.h"
#include "TrajectoryBullet.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NEO/BackGroundSystem/GunFence.h"
#include "NavigationPath.h"
#include "Kismet/KismetMathLibrary.h"


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
   
    BulletSpawnTimerHandle = FTimerHandle();
}


// Called when the game starts or when spawned
void AGunMan::BeginPlay()
{
	Super::BeginPlay();
    // プレイヤーキャラクターの参照を取得
    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    
    GetWorldTimerManager().SetTimer(BulletSpawnTimerHandle, this, &AGunMan::SpawnTrajectoryBullet, 3.0f, true);
}

FVector AGunMan::GetSnappedDirection(const FVector& Direction) const
{
    FVector SnappedDirection = Direction;

    if (FMath::Abs(SnappedDirection.X) > FMath::Abs(SnappedDirection.Y))
    {
        SnappedDirection.Y = 0.0f;
    }
    else
    {
        SnappedDirection.X = 0.0f;
    }

    return SnappedDirection.GetSafeNormal();
}
    


// Called every frame
void AGunMan::Tick(float DeltaTime)
{
 
	Super::Tick(DeltaTime);
   /* PlayerCharacter = Cast<ACharacter>(GetPlayer());
    
    {
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
            else if (DistanceToPlayer < DesiredDistance - 150.0f) // プレイヤーが望ましい距離-100以下に入った場合
            {
                // プレイヤーから離れる
                FVector PlayerDirection = GetPlayerDirection();
                AddMovementInput(-PlayerDirection);
            }
        }



        // キャラクターの位置を取得
        FVector CharacterLocation = GetActorLocation();

      
    }
*/
    if(bIsNowDamage )
    {
        return;
    }
    Super::Tick(DeltaTime);
    PlayerCharacter = Cast<ACharacter>(GetPlayer());
    if (!PlayerCharacter) return;

    float CurrentDistance = GetDistanceToPlayer();
    FVector DirectionToPlayer = GetPlayerDirection();
    FVector SnappedDirection;
    FVector MoveVector;
    

    if (CurrentDistance > DesiredDistance)
    {
        SnappedDirection = GetSnappedDirection(DirectionToPlayer);
        MoveVector = SnappedDirection * MoveSpeed * DeltaTime;
    }
    else if (CurrentDistance < DesiredDistance - 150)

    {
        SnappedDirection = GetSnappedDirection(-DirectionToPlayer);
        MoveVector = SnappedDirection * MoveSpeed * DeltaTime;
    }
    else
    {
        return; // その他の場合は移動しない
    }

    SetActorLocation(GetActorLocation() + MoveVector);
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
void AGunMan::SpawnTrajectoryBullet()
{
    // Spawn a TrajectoryBullet
    FActorSpawnParameters SpawnParams;
    FVector SpawnLocation = GetActorLocation();
    FRotator SpawnRotation = GetActorRotation(); 

    ATrajectoryBullet* Bullet = GetWorld()->SpawnActor<ATrajectoryBullet>(TrajectoryBulletClass, SpawnLocation, SpawnRotation, SpawnParams);
    if (Health > 0)
    {
        if (Bullet)
        {
            // Set up timer to replace TrajectoryBullet with Bullet after 2 seconds
            GetWorldTimerManager().SetTimer(Bullet->GetLifeSpanTimerHandle(), this, &AGunMan::ReplaceWithBullet, 2.0f, false);
        }

    }
    

    // Prevent movement for the lifespan of the TrajectoryBullet + 1 second
    GetCharacterMovement()->Deactivate();
    GetWorldTimerManager().SetTimer(MovementResumeTimerHandle, this, &AGunMan::ResumeMovement, Bullet->InitialLifeSpan + 1.0f, false);
}

void AGunMan::ReplaceWithBullet()
{
    // Replace TrajectoryBullet with Bullet
    FActorSpawnParameters SpawnParams;
    FVector SpawnLocation = GetActorLocation(); // Adjust as necessary
    FRotator SpawnRotation = GetActorRotation(); // Adjust as necessary

    ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass, SpawnLocation, SpawnRotation, SpawnParams);
    if (Bullet)
    {
        // Destroy the TrajectoryBullet if it's valid
        ATrajectoryBullet* TrajectoryBullet = Bullet->GetTrajectoryBullet();
        if (IsValid(TrajectoryBullet))
        {
            GetWorld()->DestroyActor(TrajectoryBullet);
        }
    }
}
void AGunMan::ResumeMovement()
{
    // Resume movement
    GetCharacterMovement()->Activate(true);
}
