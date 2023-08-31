// Fill out your copyright notice in the Description page of Project Settings.


#include "Rolling.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ARolling::ARolling()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	RollingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RollingSphere"));
	RollingSphere->OnComponentBeginOverlap.AddDynamic(this, &ARolling::OnOverlap);
	RollingSphere->SetupAttachment(Sphere);
}

void ARolling::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

// Called when the game starts or when spawned
void ARolling::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARolling::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

