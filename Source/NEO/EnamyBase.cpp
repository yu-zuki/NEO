// Fill out your copyright notice in the Description page of Project Settings.


#include "EnamyBase.h"
#include "GameSystem/TGS_GameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnamyBase::AEnamyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	
}

// Called every frame
void AEnamyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnamyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

