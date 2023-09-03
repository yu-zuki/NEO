// Fill out your copyright notice in the Description page of Project Settings.


#include "Pusher.h"
#include "TimerManager.h"
// Sets default values
APusher::APusher()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void APusher::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(RollingSpawnTimer, this, &APusher::SpawnRolling, 3.0f, true);

	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw = 90.0f;
	SetActorRotation(NewRotation);
}

// Called every frame
void APusher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APusher::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APusher::SpawnRolling()
{
	PlayAnimMontage(Push, 1, NAME_None);

	if (RollingToSpawn)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ARolling* SpawnedRolling = GetWorld()->SpawnActor<ARolling>(RollingToSpawn, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		if (SpawnedRolling)
		{
			FName SocketName = TEXT("enemy_L_hand");
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
			SpawnedRolling->AttachToComponent(GetMesh(), AttachmentRules, SocketName);

			CurrentRolling = SpawnedRolling;

			// 3秒後にアタッチを解除するにゃ
			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &APusher::DetachRolling, 3.0f, false);
		}

	
	}
}
void APusher::DetachRolling()
{
	if (CurrentRolling)
	{
		CurrentRolling->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		// 重力（物理シミュレーション）を有効にする
		if (CurrentRolling->Sphere)  // nullチェック
		{
			CurrentRolling->Sphere->SetSimulatePhysics(true);
			FVector ForwardForce = GetActorForwardVector() * 1000.0f;  // 適当な大きさ
			CurrentRolling->Sphere->AddForce(ForwardForce);
		}

		CurrentRolling = nullptr;  // null
	}
}