// Fill out your copyright notice in the Description page of Project Settings.


#include "HideEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "NEO/PlayerSystem/PlayerBase.h"
AHideEnemy::AHideEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// �I�u�W�F�N�g�^�O�ǉ�
	Tags.Add("Object");
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	MaxHealth = 10;
	Health = MaxHealth;
	// �ʒu�A�T�C�Y�Ȃǂ������Őݒ�ł���
	HidingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HidingBox"));
	HidingBox->SetBoxExtent(FVector(10.0f, 10.0f,10.0f)); // �T�C�Y
	HidingBox->OnComponentBeginOverlap.AddDynamic(this, &AHideEnemy::OnOverlap);
	HidingBox->SetupAttachment(Mesh);
}
void AHideEnemy::BeginPlay()
{
	Super::BeginPlay();
}


void AHideEnemy::ReceiveDamage(float DamageAmount)
{
}

void AHideEnemy::CheckAndDestroy()
{
	if (Health < 0)
	{
		if (GeometryCollectionClass)
		{
			AGeometryCollectionActor* SpawnedGeometryCollection = GetWorld()->SpawnActor<AGeometryCollectionActor>(GeometryCollectionClass, GetActorLocation(), GetActorRotation());

			if (SpawnedGeometryCollection)
			{
				FTimerHandle TimerHandle_DestroyGeometry;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_DestroyGeometry, [SpawnedGeometryCollection]()
					{
						SpawnedGeometryCollection->Destroy();
					}, 2.0f, false);
			}
		}
		// �K�v�ł���΁A�S�̂�Actor���j�󂷂�
		Destroy();
	}
}



AActor* AHideEnemy::GetPlayer()
{
	APawn* PlayerChara = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	return PlayerChara;
}

void AHideEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CheckAndDestroy();
}
void AHideEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Health > 0) 
	{
		Health -= 10;
	}
}