// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"


AEnemyAIController::AEnemyAIController()
{

}

AActor* AEnemyAIController::EnemySpawn(ASpawnPoint* spawnPoint)
{
	//NULL Check
	if (!spawnPoint) {
		UE_LOG(LogTemp, Error, TEXT("SpawnPoint is not found"));
		return nullptr;
	}

	//Transform‚ðŽæ“¾‚·‚é
	FTransform spawnTransform = spawnPoint->GetTransform();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* spawn_Actor = GetWorld()->SpawnActor<AActor>(spawnPoint->GetSpawnActorClass(), spawnPoint->GetActorLocation(), spawnPoint->GetActorRotation(), SpawnParams);


	AOdaBase* Boss = Cast<AOdaBase>(spawn_Actor);
	AEnamyBase* Enemy = Cast<AEnamyBase>(spawn_Actor);

	if (Enemy) {
		Enemy->SetActorTransform(spawnTransform);
		Enemy->IsAreaEnemy = true;		//Flag Set 

		return Enemy;
	}
	else if (Boss) {
		Boss->SetActorTransform(spawnTransform);
		Boss->IsAreaEnemy = true;		//Flag Set 


		return Boss;
	}

	return nullptr;


}