// Fill out your copyright notice in the Description page of Project Settings.


#include "AIC_Boss.h"

AAIC_Boss::AAIC_Boss()
{
}

/*
 * 関数名　　　　：SpawnEnemy()
 * 処理内容　　　：敵の出現処理
 * 引数１　　　　：出現場所の情報
 * 戻り値　　　　：なし
 */
AActor* AAIC_Boss::SpawnEnemy(ASpawnPoint* spawnPoint)
{
	//NULL Check
	if (!spawnPoint) {
		UE_LOG(LogTemp, Error, TEXT("SpawnPoint is not found"));
		return nullptr;
	}

	//Transformを取得する
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
 * 関数名　　　　：SpawnEnemyInBattleArea()
 * 処理内容　　　：バトルエリアに敵を出現させる
 * 戻り値　　　　：なし
 */
void AAIC_Boss::SpawnEnemyInBattleArea()
{
	//Check SpawnPoints
	if (BattleAreaSpawnPoints.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("SpawnPoints is not found"));
		return;
	}

	//スポーン
	UWorld* World = GetWorld();
	if (!World) return;


	for (ASpawnPoint* spawnPoint : BattleAreaSpawnPoints) {
		if (!spawnPoint) continue; //Check SpawnPoint

		//敵を生成する
		Enemies.Add(SpawnEnemy(spawnPoint));
	}
}
