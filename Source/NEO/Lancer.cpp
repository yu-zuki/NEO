#include "Lancer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

ALancer::ALancer()
{
    // キャラクターの移動方式を設
    MaxHealth = 100;
    Health = MaxHealth;
    bIsJumping = false;
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
        PlayAnimMontage(Attack, 1, NAME_None);
    }
}



