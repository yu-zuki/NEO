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

    
    MaxHealth = 100;
    Health = MaxHealth;
    bIsBulletAlive = false;
    bCanMove = true;
}


// Called when the game starts or when spawned
void AGunMan::BeginPlay()
{
	Super::BeginPlay();
    // プレイヤーキャラクターの参照を取得
    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    
    GetWorldTimerManager().SetTimer(AttackTimerHandle, this,&AGunMan::PlayAttackAnim, 5.0f, true);

    // 武器をSpawn
    if (WeaponClass && !Weapon)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, SpawnParams);

        if (Weapon)
        {
            Weapon->AttachToHand(this, "enemy_R_handSocket", EOwnerType::OwnerType_Enemy);
        }
    }
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
    
    if (bCanMove == true)
    {
        if (CurrentDistance > DesiredDistance)
        {
            SnappedDirection = GetSnappedDirection(DirectionToPlayer);
            MoveVector = SnappedDirection * MoveSpeed * DeltaTime;
        }
        else if (CurrentDistance < DesiredDistance + 400)
        {
            SnappedDirection = GetSnappedDirection(-DirectionToPlayer);
            MoveVector = SnappedDirection * MoveSpeed / 3 * DeltaTime;
        }
        else
        {
            SnappedDirection = GetSnappedDirection(DirectionToPlayer);
            MoveVector = SnappedDirection * MoveSpeed * DeltaTime;
        }

        SetActorLocation(GetActorLocation() + MoveVector);
    }
    if (Health <= 0)
    {
        if (Weapon)
        {
            Weapon->DetachToHand();
            Weapon = nullptr;
        }
    }
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
    
   
    USkeletalMeshComponent* CharacterMesh = GetMesh();
    FVector SpawnLocation = CharacterMesh->GetSocketLocation("enemy_root");
    FRotator SpawnRotation = CharacterMesh->GetSocketRotation("enemy_root");
    if (TrajectoryBulletClass)
    {
        AActor* SpawnedTrajectoryBullet = GetWorld()->SpawnActor<AActor>(TrajectoryBulletClass, SpawnLocation, SpawnRotation);
        if (SpawnedTrajectoryBullet)
        {
            SpawnedTrajectoryBullet->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "enemy_root");
         }
    }
}

void AGunMan::ReplaceWithBullet()
{
    //bIsSpawningBullet = false;
    //// Replace TrajectoryBullet with Bullet
    //FActorSpawnParameters SpawnParams;
    //FVector SpawnLocation = GetActorLocation(); // Adjust as necessary
    //FRotator SpawnRotation = GetActorRotation(); // Adjust as necessary

    //ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass, SpawnLocation, SpawnRotation, SpawnParams);
    //if (Bullet)
    //{
    //    // Destroy the TrajectoryBullet if it's valid
    //    ATrajectoryBullet* TrajectoryBullet = Bullet->GetTrajectoryBullet();
    //    if (IsValid(TrajectoryBullet))
    //    {
    //        GetWorld()->DestroyActor(TrajectoryBullet);
    //    }
    //}
    //bIsRotation = true;
    FVector SpawnBulletLocation = GetActorLocation(); // または他の場所
    FRotator SpawnBulletRotation = GetActorRotation(); // または他の回転
    if (BulletClass)
    {
        AActor* SpawnedBullet = GetWorld()->SpawnActor<AActor>(BulletClass, SpawnBulletLocation, SpawnBulletRotation);


    }
}
void AGunMan::ResumeMovement()
{
    // Resume movement
    GetCharacterMovement()->Activate(true);
   
}
void AGunMan::UnlockRotation()
{
    bIsSpawningBullet = false;
}

void AGunMan::PlayAttackAnim()
{
    if (Health > 0)
    {
        PlayAnimMontage(Attack, 1.0f, NAME_None);
       /* SetActorTickEnabled(false);*/
        GetWorldTimerManager().SetTimer(TickEnableTimerHandle, this, &AGunMan::EnableTickAfterDelay, 3.0f, false);
    }
   
}
void AGunMan::EnableTickAfterDelay()
{
    // Tickを再有効化する
    /*SetActorTickEnabled(true);*/
}