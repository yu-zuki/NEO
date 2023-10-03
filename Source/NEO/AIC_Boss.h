// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NEO/GameSystem/SpawnPoint.h"
#include "NEO/OdaBase.h"
#include "NEO/Enemy/EnamyBase.h"
#include "AIC_Boss.generated.h"

/**
 * 
 */
UCLASS()
class NEO_API AAIC_Boss : public AAIController
{
	GENERATED_BODY()

		AAIC_Boss();

public:
	//�G�̏o������
	// �G�̏o������
	AActor* SpawnEnemy(ASpawnPoint* spawnPoint);
	// �o�g���G���A���ɓG���o��������
	void SpawnEnemyInBattleArea();


	//�ϐ�
	TArray<AActor*> Enemies;
	TArray<class ASpawnPoint*> BattleAreaSpawnPoints;
	// �o�g���G���A���̓G�̃J�E���^�[
	int32 BattleAreaEnemyCount = 0;
};
