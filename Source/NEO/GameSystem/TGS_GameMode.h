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

	void SpawnPlayer(AActor* _player, FTransform _tranceform);
	void DestroyPlayer(AActor* _player);
	void RespawnPlayer();

	void SpawnEnemy(AActor* _enemy, FTransform _tranceform);
	void DestroyEnemy(AActor* _enemy);
	void ClearEnemy();

	void SetState_GameOver();

	void SetSubAction(ESubAction _eSubAction);

	UFUNCTION(BlueprintCallable, Category = "Area")
		void SetIsOnBattleArea(bool bIsOnArea, AActor* Camera,class UProceduralMeshComponent* LeftMesh, 
			class UProceduralMeshComponent* RightMesh,	class UProceduralMeshComponent* NearMesh);
	
	UFUNCTION(BlueprintCallable, Category = "Area")
		bool GetIsOnBattleArea();

private:
	class ATGS_GameStateBase* GetGameState();

};
