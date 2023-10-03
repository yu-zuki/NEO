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
	//敵の出現消去
	// 敵の出現処理
	AActor* SpawnEnemy(ASpawnPoint* spawnPoint);
	// バトルエリア内に敵を出現させる
	void SpawnEnemyInBattleArea();


	//変数
	TArray<AActor*> Enemies;
	TArray<class ASpawnPoint*> BattleAreaSpawnPoints;
	// バトルエリア内の敵のカウンター
	int32 BattleAreaEnemyCount = 0;
};
