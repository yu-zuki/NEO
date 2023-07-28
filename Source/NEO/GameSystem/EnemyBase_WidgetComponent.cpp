// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase_WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "EnemyBase_UMG.h"
#include "TGS_GameMode.h"
#include "Kismet/GameplayStatics.h"

void UEnemyBase_WidgetComponent::BeginPlay()
{
	Super::BeginPlay();


	SetWidgetSpace(EWidgetSpace::World);
	if (WidgetClass != nullptr) {
		SetWidgetClass(WidgetClass);
	}
}

void UEnemyBase_WidgetComponent::LookAtPlayer()
{
	ATGS_GameMode* GameMode = Cast<ATGS_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode)
	{
	// Camera To Enemy
	FVector CameraToEnemy = GameMode->GetCameraLocation() - GetOwner()->GetActorLocation();

	CameraToEnemy.Y = 0;
	CameraToEnemy.Z = 0;	
	CameraToEnemy.Normalize();

	FRotator LookAtRotation = CameraToEnemy.Rotation();
	SetWorldRotation(LookAtRotation);
	return;		
	}
}

void UEnemyBase_WidgetComponent::SetHPInfo(float HP, float MaxHP)
{
	if (WidgetClass != nullptr) {
		//Get Character Widget
		UEnemyBase_UMG* CharacterWidget = Cast<UEnemyBase_UMG>(GetWidget());
		if (CharacterWidget != nullptr) {
			//Set HP
			CharacterWidget->SetPlayerInfo(HP, MaxHP);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("CharacterWidget is nullptr"));
		}
	}
}

void UEnemyBase_WidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	LookAtPlayer();
}
