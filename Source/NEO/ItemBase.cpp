// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// SceneComponent���쐬����
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	// SceneComponent��RootComponent�ɐݒ肷��
	RootComponent = DefaultSceneRoot;

	// StaticMeshComponent���쐬����
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// StaticMesh��Laod����StaticMeshComponent��StaticMesh�ɐݒ肷��
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(NULL, TEXT("/Script/Engine.StaticMesh'/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube'"), NULL, LOAD_None, NULL);
	StaticMesh->SetStaticMesh(Mesh);

	// StaticMeshComponent��RootComponent��Attach����
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

