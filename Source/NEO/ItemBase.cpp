// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// SceneComponent‚ğì¬‚·‚é
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	// SceneComponent‚ğRootComponent‚Éİ’è‚·‚é
	RootComponent = DefaultSceneRoot;

	// StaticMeshComponent‚ğì¬‚·‚é
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// StaticMesh‚ğLaod‚µ‚ÄStaticMeshComponent‚ÌStaticMesh‚Éİ’è‚·‚é
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(NULL, TEXT("/Script/Engine.StaticMesh'/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube'"), NULL, LOAD_None, NULL);
	StaticMesh->SetStaticMesh(Mesh);

	// StaticMeshComponent‚ğRootComponent‚ÉAttach‚·‚é
	StaticMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

