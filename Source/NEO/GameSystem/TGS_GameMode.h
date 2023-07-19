// 更新日：2023/6/5		更新者：董		更新内容：ゲームモードの基本クラスを作成
// 更新日：2023/6/8		更新者：董		更新内容：ゲームモードのKeyInput機能、基本的な機能を作成

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InputCharacter.h"

#include "TGS_GameMode.generated.h"

enum class EChangeLevel : uint8;

/**
 *
 */
UCLASS()
class NEO_API ATGS_GameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ATGS_GameMode();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameClass")
		TSubclassOf<class AActor> PlayerBaseClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameClass")
		TSubclassOf<class AActor> EnemyBaseClass;


public:
	void BeginPlay() override;

	void StartPlay() override;

	void Tick(float DeltaTime) override;

	void NextLevel(FName _nextLevelName);

	UFUNCTION(BlueprintCallable, Category = "Level")
		void ChangeNextLevel(ENextLevel NextLevel);					//目標のLevelに遷移する

//---------------------------------------スポーン＆デストロイ---------------------------------------
	void SpawnPlayer(AActor* _player, FTransform _tranceform);
	void DestroyPlayer(AActor* _player);
	void RespawnPlayer();

	//バトルエリアにいる敵をスポーンする
	void SpawnEnemyInBattleArea();

	void SpawnEnemy(AActor* _enemy, FTransform _tranceform);
	AActor* SpawnEnemy(ASpawnPoint* spawnPoint);

	UFUNCTION(BlueprintCallable, Category = "GameSystem")
	void DestroyEnemy(AActor* _enemy, bool BattleAreaEnemy = false);

	void ClearEnemy();

//--------------------------------------------キー入力---------------------------------------------
	void SetState_GameOver();

	void SetSubAction(ESubAction _eSubAction);

//-----------------------------------------バトルエリア-------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "Area")
		void SetIsOnBattleArea(bool bIsOnArea, TArray<class ASpawnPoint*> SpawnPoints, 
			AActor* Camera,
			class UProceduralMeshComponent* LeftMesh,
			class UProceduralMeshComponent* RightMesh, 
			class UProceduralMeshComponent* NearMesh
			);
	
	UFUNCTION(BlueprintCallable, Category = "Area")
		bool GetIsOnBattleArea();

	int32 GetBattleAreaEnemyNum();

//---------------------------------------ゲームUI---------------------------------------
	void SetUI_Enemy(FName _ActorName, int32 _NowHp, int32 _MaxHp);

protected:
	UFUNCTION(BlueprintCallable, Category = "GameSystem")
	class ATGS_GameStateBase* GetGameState();

	ATGS_GameStateBase* GameState = nullptr;		//キャシュー
		
};
