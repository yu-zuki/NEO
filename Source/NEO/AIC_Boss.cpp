// Fill out your copyright notice in the Description page of Project Settings.


#include "AIC_Boss.h"

AAIC_Boss::AAIC_Boss()
{
}

/*
 * �֐����@�@�@�@�FSpawnEnemy()
 * �������e�@�@�@�F�G�̏o������
 * �����P�@�@�@�@�F�o���ꏊ�̏��
 * �߂�l�@�@�@�@�F�Ȃ�
 */
AActor* AAIC_Boss::SpawnEnemy(ASpawnPoint* spawnPoint)
{
	//NULL Check
	if (!spawnPoint) {
		UE_LOG(LogTemp, Error, TEXT("SpawnPoint is not found"));
		return nullptr;
	}

	//Transform���擾����
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


/*
 * �֐����@�@�@�@�FSpawnEnemyInBattleArea()
 * �������e�@�@�@�F�o�g���G���A�ɓG���o��������
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void AAIC_Boss::SpawnEnemyInBattleArea()
{
	//Check SpawnPoints
	if (BattleAreaSpawnPoints.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("SpawnPoints is not found"));
		return;
	}

	//�X�|�[��
	UWorld* World = GetWorld();
	if (!World) return;


	for (ASpawnPoint* spawnPoint : BattleAreaSpawnPoints) {
		if (!spawnPoint) continue; //Check SpawnPoint

		//�G�𐶐�����
		Enemies.Add(SpawnEnemy(spawnPoint));
	}
}
