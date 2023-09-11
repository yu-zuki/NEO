#include "Lancer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "NEO/WeaponSystem/WeaponBase.h"


ALancer::ALancer()
{
    // キャラクターの移動方式を設
    MaxHealth = 100;
    Health = MaxHealth;
    bIsRandMove = false;
    // キャラクターの移動方式を設定
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
  
}



void ALancer::BeginPlay()
{
    Super::BeginPlay();

    // プレイヤーキャラクターの参照を取得
    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    // 5秒ごとにCheckPlayerInFront関数を実行するタイマーをセット
    GetWorldTimerManager().SetTimer(TimerHandle_CheckPlayerInFront, this, &ALancer::CheckPlayerInFront, 3.0f, true);
    GetWorld()->GetTimerManager().SetTimer(MoveToTargetTimer, this, &ALancer::ChooseNewTarget, 3.0f, true);
    SpawnTime = GetWorld()->GetTimeSeconds();
    // 武器をSpawn
    if (WeaponClass && !Weapon)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, SpawnParams);

        if (Weapon)
        {
            Weapon->AttachToHand(this, "enemy_R_handSocket");
        }

        Weapon->SetOwnerType(EOwnerType::OwnerType_Enemy);
    }
}

FVector ALancer::GetSnappedDirection(const FVector& Direction) const
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

void ALancer::CollisionOn()
{
    if (Weapon)
    {
        Weapon->SetCollision();
    }
}
void ALancer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bIsNowDamage || bShouldSkipNextMovement)
    {
        bShouldSkipNextMovement = false;
        return;
    }
    
   

   
    PlayerCharacter = Cast<ACharacter>(GetPlayer());
    if (!PlayerCharacter) return;

    float CurrentDistance = GetDistanceToPlayer();
    FVector DirectionToPlayer = GetPlayerDirection();
    FVector SnappedDirection;
    FVector MoveVector;
    

    if (CurrentDistance <= DesiredDistance + 300)
    {
        bIsRandMove = true;
    }
    else if(CurrentDistance > DesiredDistance - 100)
    {
        bIsRandMove = false;
    }
    if (Health > 0&& IsAnimationAttacking()==false)
    {

        //if (CurrentDistance < DesiredDistance + 400 && TimeUntilNextRandomMove <= 0) // DesiredDistanceより400m以上離れている場合
        //{
        //    // ランダムに動く方向を決定
        //    FVector RandomDirection = FVector(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), 0.0f).GetSafeNormal();
        //    MoveVector = RandomDirection * MoveSpeed * DeltaTime;

        //    // 次のランダムな移動までの待機時間を設定する
        //    TimeUntilNextRandomMove = FMath::RandRange(MinWaitTime, MaxWaitTime);
        //}
       /* else */
        if (CurrentDistance > DesiredDistance&& bIsRandMove==false)
        {
            SnappedDirection = GetSnappedDirection(DirectionToPlayer);
            MoveVector = SnappedDirection * Speed * DeltaTime;
           
        }
       else if(CurrentDistance < DesiredDistance + 400 && CurrentTarget&& bIsRandMove==true) // DesiredDistanceより400m遠い場合
       {
           FVector DirectionToTarget = (CurrentTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
           MoveVector = DirectionToTarget * Speed * DeltaTime;
           
       }

       /* else if (CurrentDistance < DesiredDistance +400)
        {
            SnappedDirection = GetSnappedDirection(-DirectionToPlayer);
            MoveVector = SnappedDirection * MoveSpeed/2 * DeltaTime;
        }*/
    else
    {
            /*FVector RandomDirection = FVector(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), 0.0f).GetSafeNormal();
            MoveVector = RandomDirection * MoveSpeed * DeltaTime+10;*/

            // 次のランダムな移動までの待機時間を設定する
          ///* ///* TimeUntilNextRandomMove = FMath::RandRange(MinWaitTime, MaxWaitTime*/);*/
            return; // その他の場合は移動しない
    }
    }
    if (Health <= 0)
    {
        Weapon->DetachToHand();
    }
 
    SetActorLocation(GetActorLocation() + MoveVector);
}
void ALancer::ChooseNewTarget()
{
    TArray<AActor*> FoundTargetPoints;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Random"), FoundTargetPoints);

    if (FoundTargetPoints.Num() > 0)
    {
        // ランダムなTargetPointを選択する
        CurrentTarget = Cast<ATargetPoint>(FoundTargetPoints[FMath::RandRange(0, FoundTargetPoints.Num() - 1)]);
    }
}

FVector ALancer::GetPlayerDirection() const
{
    if (!PlayerCharacter) return FVector::ZeroVector;

    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector LancerLocation = GetActorLocation();
    return FVector(PlayerLocation.X - LancerLocation.X, PlayerLocation.Y - LancerLocation.Y, 0.0f).GetSafeNormal();
}
float ALancer::GetDistanceToPlayer() const
{
    if (!PlayerCharacter) return 0.0f;

    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector LancerLocation = GetActorLocation();
    return FVector::Distance(PlayerLocation, LancerLocation);
}

void ALancer::CheckPlayerInFront()
{
    // 自分の位置を取得
    FVector MyLocation = GetActorLocation();
    
    if (bIsNowDamage || bShouldSkipNextMovement)
    {
        bShouldSkipNextMovement = false;
        return;
    }
        UWorld* World = GetWorld();
        if (World)
        {
            APlayerController* PlayerController = World->GetFirstPlayerController();
            if (PlayerController)
            {
                APawn* Pawn = PlayerController->GetPawn();
                if (Pawn)
                {
                    FVector PlayerLocation = Pawn->GetActorLocation();
                    // 自プレイヤーがLancerの目の前にいるかどうかを判定
                    FVector DirectionToPlayer = PlayerLocation - MyLocation;
                    float DotProduct = FVector::DotProduct(DirectionToPlayer.GetSafeNormal(), GetActorForwardVector());
                    bIsPlayerInFront = DotProduct > 0.0f;

                    if (bIsPlayerInFront&& Health > 0)
                    {
                        if (FMath::FRand() < 0.8f)
                        {
                            PlayAnimMontage(Attack, 1, NAME_None);
                              
                        }

                    }
                }
            }
        }
    
   
    
}

//FVector ALancer::RoundDirectionT45Degrees(FVector direction) const
//{
//   
//    float angle = FMath::Atan2(direction.Y, direction.X);
//
//  
//    float angleDegrees = FMath::RadiansToDegrees(angle);
//
//   
//    float roundedAngleDegrees = FMath::RoundToFloat(angleDegrees / 45.0f) * 45.0f;
//
//   
//    float roundedAngle = FMath::DegreesToRadians(roundedAngleDegrees);
//
//   
//    FVector roundedDirection(FMath::Cos(roundedAngle), FMath::Sin(roundedAngle), 0.0f);
//
//    return roundedDirection;
//}










