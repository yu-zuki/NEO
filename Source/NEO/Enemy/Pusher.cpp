// Fill out your copyright notice in the Description page of Project Settings.


#include "Pusher.h"
#include "TimerManager.h"
// Sets default values
APusher::APusher()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void APusher::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(RollingSpawnTimer, this, &APusher::SpawnRolling, 3.0f, true);
}

// Called every frame
void APusher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APusher::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APusher::SpawnRolling()
{
	PlayAnimMontage(Push, 1, NAME_None);

	if (RollingToSpawn)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ARolling* SpawnedRolling = GetWorld()->SpawnActor<ARolling>(RollingToSpawn, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		if (SpawnedRolling)
		{
			FName SocketName = TEXT("enemy_L_hand");
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
			SpawnedRolling->AttachToComponent(GetMesh(), AttachmentRules, SocketName);
		}

	
	}
}