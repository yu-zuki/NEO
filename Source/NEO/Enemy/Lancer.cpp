#include "Lancer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

ALancer::ALancer()
{
    // キャラクターの移動方式を設
    MaxHealth = 100;
    Health = MaxHealth;
   
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


void ALancer::Tick(float DeltaTime)
{
    if (bIsNowDamage || bShouldSkipNextMovement)
    {
        bShouldSkipNextMovement = false;
        return;
    }
    Super::Tick(DeltaTime);
    PlayerCharacter = Cast<ACharacter>(GetPlayer());
    if (!PlayerCharacter) return;

    float CurrentDistance = GetDistanceToPlayer();
    FVector DirectionToPlayer = GetPlayerDirection();
    FVector SnappedDirection;
    FVector MoveVector;

    if (Health > 0)
    {
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
    }
   
    //移動中アニメーション再生
    if (MoveVector.Size() > 0 && MovingAnimation && AnimInstance)
    {
        if (!AnimInstance->Montage_IsPlaying(MovingAnimation))
        {
            AnimInstance->Montage_Play(MovingAnimation);
        }
        else if (MovingAnimation && AnimInstance)
        {
            AnimInstance->Montage_Stop(0.2f, MovingAnimation);
        }
    }
    SetActorLocation(GetActorLocation() + MoveVector);
}
void ALancer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
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
                        if (FMath::FRand() < 0.5f)
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










