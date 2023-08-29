// Fill out your copyright notice in the Description page of Project Settings.


#include "Destroyer.h"

// Sets default values
ADestroyer::ADestroyer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh1"));
	RootComponent = Mesh1;
	SkeletalMesh1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh1"));
	SkeletalMesh1->SetupAttachment(RootComponent);

	SkeletalMesh2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh2"));
	SkeletalMesh2->SetupAttachment(RootComponent);

	SkeletalMesh3 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh3"));
	SkeletalMesh3->SetupAttachment(RootComponent);

	SkeletalMesh4 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh4"));
	SkeletalMesh4->SetupAttachment(RootComponent);

	SkeletalMesh5 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh5"));
	SkeletalMesh5->SetupAttachment(RootComponent);

	SkeletalMesh6 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh6"));
	SkeletalMesh6->SetupAttachment(RootComponent);
}


// Called when the game starts or when spawned
void ADestroyer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADestroyer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADestroyer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

