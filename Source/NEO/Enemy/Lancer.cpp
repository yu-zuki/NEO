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
    MyEnemyDamage = nullptr;
}



void ALancer::BeginPlay()
{
    Super::BeginPlay();

    // プレイヤーキャラクターの参照を取得
    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    // 5秒ごとにCheckPlayerInFront関数を実行するタイマーをセット
    GetWorldTimerManager().SetTimer(TimerHandle_CheckPlayerInFront, this, &ALancer::CheckPlayerInFront, 3.0f, true);
}


void ALancer::Tick(float DeltaTime)
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
    }
    
   
   UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && Attack && AnimInstance->Montage_IsPlaying(Attack))
    {
        
        EnableCollisionWhenMontagePlaying();
    }
    else
    {
       
        DisableCollisionWhenMontageNotPlaying();
    }
   
   
   
   
    
    
    
}
void ALancer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
FVector ALancer::GetPlayerDirection() const
{
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector LancerLocation = GetActorLocation();
    return FVector(PlayerLocation.X - LancerLocation.X, PlayerLocation.Y - LancerLocation.Y, 0.0f).GetSafeNormal();
}
float ALancer::GetDistanceToPlayer() const
{
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector LancerLocation = GetActorLocation();
    return FVector::Distance(PlayerLocation, LancerLocation);
}

void ALancer::CheckPlayerInFront()
{
    // 自分の位置を取得
    FVector MyLocation = GetActorLocation();

    // 自プレイヤーの位置を取得
    FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

    // 自プレイヤーがLancerの目の前にいるかどうかを判定
    FVector DirectionToPlayer = PlayerLocation - MyLocation;
    float DotProduct = FVector::DotProduct(DirectionToPlayer.GetSafeNormal(), GetActorForwardVector());
    bIsPlayerInFront = DotProduct > 0.0f;

    if (bIsPlayerInFront)
    {
        if (FMath::FRand()<0.25f)
        {
             PlayAnimMontage(Attack, 1, NAME_None);
        }
      
    }
}


void ALancer::EnableCollisionWhenMontagePlaying()
{
    if (MyEnemyDamage)
    {
        UBoxComponent* BoxComponent = Cast<UBoxComponent>(MyEnemyDamage->GetRootComponent());
        if (BoxComponent)
        {
            BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        }
    }
}


void ALancer::DisableCollisionWhenMontageNotPlaying()
{
    if (MyEnemyDamage)
    {
        UBoxComponent* BoxComponent = Cast<UBoxComponent>(MyEnemyDamage->GetRootComponent());
        if (BoxComponent)
        {
            BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
    }
}





