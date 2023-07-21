// Fill out your copyright notice in the Description page of Project Settings.


#include "EnamyBase.h"
#include "NEO/GameSystem/TGS_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "NEO/GameSystem/Enemy_UMG.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "NEO/GameSystem/EnemyBase_WidgetComponent.h"

// Sets default values
AEnamyBase::AEnamyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
<<<<<<< HEAD
    InitialSpeed = 1000.0f;
    LaunchAngle = 45.0f;
    bIsProjectile = false;
    Gravity = 9.8f;

=======
    
>>>>>>> 895caac45da460713c5ce2d513d4c1ebd8f07b96
	//UI Create
	EnemyWidget = CreateDefaultSubobject<UEnemyBase_WidgetComponent>(TEXT("EnemyWidget"));
	EnemyWidget->SetupAttachment(RootComponent);
}

void AEnamyBase::DestoryEnemy()
{
	ATGS_GameMode* GameMode =Cast<ATGS_GameMode>( UGameplayStatics::GetGameMode(GetWorld()) );
	if (GameMode)
	{
		GameMode->DestroyEnemy(this,IsAreaEnemy);
	}
}

// Called when the game starts or when spawned
void AEnamyBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnDefaultController();

<<<<<<< HEAD
    float rad = FMath::DegreesToRadians(LaunchAngle);
    InitialDirection = FVector(cos(rad), 0, sin(rad));
=======
 
    
>>>>>>> 895caac45da460713c5ce2d513d4c1ebd8f07b96
}

// Called every frame
void AEnamyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    // キャラクターの位置を取得
    FVector CharacterLocation = GetActorLocation();

    // 自分の座標を取得
    FVector MyLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	//Enemy Hp Set
	EnemyWidget->SetHPInfo(Health, MaxHealth);
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
<<<<<<< HEAD
    if (bIsProjectile)
    {
        ProjectileMotion(DeltaTime);
   }
=======
>>>>>>> 895caac45da460713c5ce2d513d4c1ebd8f07b96


}

// Called to bind functionality to input
void AEnamyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AEnamyBase::ApplyDamage(float DamageAmount)
{
	Health -= DamageAmount;
    if (Health <= 0)
    {
<<<<<<< HEAD
        
        bIsProjectile = true;
=======
        PlayAnimMontage(Death, 1, NAME_None);
>>>>>>> 895caac45da460713c5ce2d513d4c1ebd8f07b96
    }
}

void AEnamyBase::ProjectileMotion(float DeltaTime)
{
    Time += DeltaTime;
    FVector CurrentVelocity = InitialSpeed * InitialDirection * Time;
    CurrentVelocity.Z -= 0.5 * Gravity * Time * Time;
    FVector NewLocation = GetActorLocation() + CurrentVelocity * DeltaTime;
    SetActorLocation(NewLocation);
}



