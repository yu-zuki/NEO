// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NEO/GameSystem/SpawnPoint.h"
#include "OdaBase.h"
#include "Enemy/EnamyBase.h"
#include "Templates/SubclassOf.h"								//TSubclassOfを使うために必要なヘッダー
#include "EnemyAIController.generated.h"

/**
 *
 */
UCLASS()
class NEO_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
		AEnemyAIController();
	//やること
	//敵の出現削除
	//ボス、敵のマップ移動時処理

	//関数
	//敵の出現------------------------------------------------
		AActor* EnemySpawn(ASpawnPoint* spawnPoint);



	//変数
	//敵の出現------------------------------------------------
	UPROPERTY(EditAnywhere)
	TSubclassOf< class AActor >EnemyInfo = nullptr;


};
