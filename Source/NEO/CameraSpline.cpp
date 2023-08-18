// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/SpringArmComponent.h"
#include "CameraSpline.h"

// Sets default values
ACameraSpline::ACameraSpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));


}

// Called when the game starts or when spawned
void ACameraSpline::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

