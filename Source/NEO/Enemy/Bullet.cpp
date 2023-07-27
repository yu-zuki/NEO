// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "NEO/PlayerSystem/PlayerBase.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (!SphereComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
	if (!SphereComponent)
	{
		// Use a sphere as a simple collision representation.
		SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// Set the sphere's collision radius.
		SphereComponent->InitSphereRadius(15.0f);
		
	}
	

	

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	MovementDirection = GetActorForwardVector();
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector StartLocation = GetActorLocation();
	FVector CurrentLocation = StartLocation;
	float DistanceTraveled = FVector::Dist(CurrentLocation, StartLocation);

	// Calculate the speed based on distance
	float CurrentSpeed = Speed * FMath::Max(0.0f, 1.0f - DistanceTraveled * DistanceSpeedDamping);

	// Update the position of the bullet based on the current speed
	FVector NewLocation = CurrentLocation + (MovementDirection * CurrentSpeed * DeltaTime);
	SetActorLocation(NewLocation);
}

