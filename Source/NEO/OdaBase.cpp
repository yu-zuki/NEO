// Fill out your copyright notice in the Description page of Project Settings.


#include "OdaBase.h"


// Sets default values
AOdaBase::AOdaBase():
	OdaNobunaga(this),
	NobunagaMovement(OdaNobunaga->GetCharacterMovement()),
	ForwardDirection(OdaNobunaga->GetActorForwardVector()),
	OdaSpeed(1.f)

{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOdaBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOdaBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	OdaNobunaga->AddMovementInput(ForwardDirection, OdaSpeed);
}

// Called to bind functionality to input
void AOdaBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

