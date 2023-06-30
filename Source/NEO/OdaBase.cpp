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
	ChangeFlontTimer(20),
	OdaMoveEnum(ECPPOdaEnum::Stay1),
	SwitchStayMove(true),
	WaitTime(0)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AOdaBase::BeginPlay()
{
	Super::BeginPlay();
}


// Called to bind functionality to input
void AOdaBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


// Called every frame
void AOdaBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation()));
	ForwardDirection = OdaNobunaga->GetActorForwardVector();

		FlontTimer += 1;
		WaitTime += 1;
		switch (OdaMoveEnum)
		{
		case ECPPOdaEnum::Stay1:
			OdaStay1(FlontTimer);
			break;

		case ECPPOdaEnum::Move1:
			OdaMove1(FlontTimer);
			break;

		case ECPPOdaEnum::Move2:
			OdaMove2(FlontTimer);
			break;		

		case ECPPOdaEnum::Back1:
			OdaBack1(FlontTimer);
			break;
		default:

			break;
		}

}
void AOdaBase::OdaStay1(int Timer)
{
	if (Timer % 60 == 0)
	{
		SwitchStayMove = !SwitchStayMove;
	}
	if (SwitchStayMove)
	{
		FlontMove(OdaSpeed / 10.f);
	}
	else
	{
		BackMove(OdaSpeed / 10.f);
	}

	if (GetDistanceTo(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) >= 400 && WaitTime >= 60)
	{
		if (Timer % 2 == 0)
		{
			OdaMoveEnum = ECPPOdaEnum::Move1;
			SwitchStayMove = false;
		}
		else
		{
			OdaMoveEnum = ECPPOdaEnum::Move2;
			SwitchStayMove = false;
		}
	}
	else if (GetDistanceTo(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) <= 300 && WaitTime >= 60)
	{
		OdaMoveEnum = ECPPOdaEnum::Back1;
		SwitchStayMove = false;
		WaitTime = 0;
	}
}

void AOdaBase::OdaMove1(int Timer)
{
	if (Timer % 60 == 0)
	{
		SwitchStayMove = true;
	}
	if (SwitchStayMove)
	{
		FlontMove(FastOdaSpeed*2);
	}
	else
	{ 
		BackMove(OdaSpeed / 5.f);
	}
	if (GetDistanceTo(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) <= 250) {
		OdaMoveEnum = ECPPOdaEnum::Back1;
		SwitchStayMove = false;
		WaitTime = 0;

	}
}

void AOdaBase::OdaMove2(int Timer)
{
	if (Timer % 40 == 0)
	{
		SwitchStayMove = !SwitchStayMove;
	}
	if (SwitchStayMove)
	{
		FlontMove(FastOdaSpeed*4.f);
	}
	else
	{
		BackMove(OdaSpeed / 10.f);
	}
	if (GetDistanceTo(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) <= 250) {
		OdaMoveEnum = ECPPOdaEnum::Back1;
		SwitchStayMove = false;
		WaitTime = 0;

	}
}

void AOdaBase::OdaBack1(int Timer) {
	if (Timer % 90 == 0)
	{
		OdaMoveEnum = ECPPOdaEnum::Stay1;
		SwitchStayMove = false;
		WaitTime = 0;
	}
	BackMove(FastOdaSpeed * 10.f);

}



void AOdaBase::FlontMove(float Speed)
{
	OdaNobunaga->AddMovementInput(ForwardDirection, Speed);
}

void AOdaBase::BackMove(float Speed)
{
	OdaNobunaga->AddMovementInput(ForwardDirection, -Speed);
}
