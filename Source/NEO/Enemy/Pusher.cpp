// Fill out your copyright notice in the Description page of Project Settings.


#include "Pusher.h"

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

