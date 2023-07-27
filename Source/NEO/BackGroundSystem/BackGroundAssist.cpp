// Fill out your copyright notice in the Description page of Project Settings.


#include "BackGroundAssist.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NEO/PlayerSystem/CharacterCamera.h"
#include "Camera/CameraComponent.h"
#include "NEO/GameSystem/TGS_GameMode.h"

#define DIRECTION (90.0)

// Sets default values for this component's properties
UBackGroundAssist::UBackGroundAssist()
	: bUseBillboard(true)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UBackGroundAssist::BeginPlay()
{
	Super::BeginPlay();


}


// Called every frame
void UBackGroundAssist::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 常にカメラを向く
	ToFaceCamera();
}

void UBackGroundAssist::ToFaceCamera()
{
	// 機能のオン・オフ
	if (!bUseBillboard) { return; }

	ATGS_GameMode* GameMode = Cast<ATGS_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode)
	{
		// Camera To Enemy
		FVector CameraToEnemy = GetOwner()->GetActorLocation() - GameMode->GetCameraLocation();
		CameraToEnemy.Y = 0;
		CameraToEnemy.Z = 0;

		FRotator LookAtRotation = CameraToEnemy.Rotation();
		GetOwner()->SetActorRotation(FRotator(LookAtRotation.Pitch, LookAtRotation.Yaw, LookAtRotation.Roll));

	}
}