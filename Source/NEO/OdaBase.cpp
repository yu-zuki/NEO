// Fill out your copyright notice in the Description page of Project Settings.


#include "OdaBase.h"


// Sets default values
AOdaBase::AOdaBase():
	OdaNobunaga(this),
	NobunagaMovement(OdaNobunaga->GetCharacterMovement()),
	ForwardDirection(OdaNobunaga->GetActorForwardVector()),
	OdaSpeed(1.f),
	FastOdaSpeed(1.65),
	FlontTimer(0),
	ChangeFlontTimer(20)

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
	if (GetDistanceTo(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) >= 300)
	{
		FlontTimer += 1;
		if (FlontTimer <= ChangeFlontTimer)
		{
			FlontMove(OdaSpeed);
		}
		else
		{
			FlontMove(FastOdaSpeed);
			FlontTimer = 0;
		}
	}
	else
	{
		BackMove(OdaSpeed);
	}
}

// Called to bind functionality to input
void AOdaBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void AOdaBase::FlontMove(float Speed)
{
	OdaNobunaga->AddMovementInput(ForwardDirection, Speed);
}


void AOdaBase::BackMove(float Speed)
{
	OdaNobunaga->AddMovementInput(ForwardDirection, -Speed);
}
