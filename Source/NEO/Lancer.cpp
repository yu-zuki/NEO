#include "Lancer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

ALancer::ALancer()
{
    // キャラクターの移動方式を設
    MaxHealth = 100;
    Health = MaxHealth;
    bIsJumping = false;
    //重力加速度
    fGravityAcceleration = 9.8f;
    //ジャンプ時間
    fJumpTime = 0.0f;
    //ジャンプ高さ
    fJumpHeight = 100.0f;
    //ジャンプ開始時の位置
    vJumpStartLocation = GetActorLocation();
    
    
}



void ALancer::BeginPlay()
{
    Super::BeginPlay();

    // プレイヤーキャラクターの参照を取得
    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
}

void ALancer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // Make the character's Y-axis always face towards the camera/player
    if (GEngine && GEngine->GetFirstLocalPlayerController(GetWorld()))
    {
        FVector PlayerLocation = GEngine->GetFirstLocalPlayerController(GetWorld())->GetPawn()->GetActorLocation();
        FVector CharacterLocation = GetActorLocation();
        FVector DirectionToPlayer = PlayerLocation - CharacterLocation;
        FRotator NewRotation = DirectionToPlayer.Rotation();
        NewRotation.Pitch = 0.f;
        NewRotation.Roll = 0.f;
        NewRotation.Yaw +=180.f;
        SetActorRotation(NewRotation);
        FVector CharacterForward = GetActorForwardVector();
        float DotProduct = FVector::DotProduct(CharacterForward, DirectionToPlayer.GetSafeNormal());
        if (DotProduct < 0.f)
        {
            // Rotate Y-axis by 180 degrees
            NewRotation.Yaw += 180.f;
        }
    }
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
void ALancer::ApplyDamage(float DamageAmount,float DeltaTime)
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
void ALancer::StartJumpByGravity(float JumpHeight, float GravityAcceleration)
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
void ALancer::JumpingByGravity(float DeltaTime)
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
void ALancer::EndJumpByGravity()
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
void ALancer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}





