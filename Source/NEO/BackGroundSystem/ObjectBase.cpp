// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectBase.h"
#include "GameFramework/Character.h"
#include "NEO/PlayerSystem/CharacterCamera.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AObjectBase::AObjectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

}

// Called when the game starts or when spawned
void AObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

AActor* AObjectBase::GetPlayer()
{
	APawn* PlayerChara = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	return PlayerChara;
}

// Called every frame
void AObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PlayerCharacter = Cast<ACharacter>(GetPlayer());
	/*
	float DistanceTolayer = FVector::Dist(PlayerCharacter->GetActorLocation(), GetActorLocation());
	int32 LodLevel;
	if (DistanceTolayer < 500.0f)
	{
		LodLevel = 1;
	}
	else if(DistanceTolayer < 1000.0f)
	{
		LodLevel = 2;
	}
	else
	{
		LodLevel = 3;
	}
	Mesh->SetForcedLodModel(LodLevel);

	*/
}


