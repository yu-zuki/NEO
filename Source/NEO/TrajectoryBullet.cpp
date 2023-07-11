// Fill out your copyright notice in the Description page of Project Settings.


#include "TrajectoryBullet.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
// Sets default values
ATrajectoryBullet::ATrajectoryBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    BulletMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMeshComponent"));
    RootComponent = BulletMeshComponent;

    
    MinOpacity = 0.0f;
    MaxOpacity = 1.0f;
    BlinkDuration = 2.0f;
}

// Called when the game starts or when spawned
void ATrajectoryBullet::BeginPlay()
{
	Super::BeginPlay();
    if (BulletMaterial)
    {
        DynamicMaterialInstance = UMaterialInstanceDynamic::Create(BulletMaterial, this);
        if (DynamicMaterialInstance)
        {
            BulletMeshComponent->SetMaterial(0, DynamicMaterialInstance);
        }
    }
    
   
}

// Called every frame
void ATrajectoryBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrajectoryBullet::StartBlinking()
{
    if (DynamicMaterialInstance)
    {
      
        SetBulletOpacity(MaxOpacity);

       
        GetWorldTimerManager().SetTimer(BlinkTimerHandle, this, &ATrajectoryBullet::BlinkBullet, BlinkDuration, true);
    }
}

void ATrajectoryBullet::SetBulletOpacity(float Opacity)
{
    if (DynamicMaterialInstance)
    {
        DynamicMaterialInstance->SetScalarParameterValue(TEXT("Opacity"), Opacity);
    }
}

void ATrajectoryBullet::BlinkBullet()
{
   
    // Toggle the bullet opacity between the minimum and maximum values
    FName OpacityParamName = FName("Opacity");
    float CurrentOpacity;
    DynamicMaterialInstance->GetScalarParameterValue(OpacityParamName, CurrentOpacity);

    float NewOpacity = (CurrentOpacity == MinOpacity) ? MaxOpacity : MinOpacity;
    SetBulletOpacity(NewOpacity);
}
void ATrajectoryBullet::DeleteBullet()
{
    Destroy();
}