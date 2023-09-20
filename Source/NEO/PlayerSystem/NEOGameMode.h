// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NEOGameMode.generated.h"

UCLASS()
class NEO_API ANEOGameMode : public AGameModeBase
{
	GENERATED_BODY()

	ANEOGameMode();

	// ゲーム開始時の処理
	void BeginPlay() override;

	// 毎フレーム更新
	void Tick(float DeltaTime) override;

public:

	UFUNCTION(BlueprintCallable, Category = "UpdateState")
		class ANEOGameState* GetGameState() { return pGameState; }

	// プレイヤーのカメラ変更
	void SetViewTargetWithBlend(AActor* _newViewTarget, float _blendTime = 0.f, EViewTargetBlendFunction _blendFunc = VTBlend_Linear, float _blendExp = 0.f, bool _bLockOutgoing = false);

	// プレイヤーのカメラ設定
	void SetPlayerCamera(AActor* _playerCamera) { PlayerCamera = _playerCamera; }
	
	// 現在のカメラ取得
	AActor* GetPlayerCamera()const { return pCamera; }

	FRotator GetCameraRotation()const;

	UFUNCTION(BlueprintCallable, Category = "Area")
		void SetIsOnBattleArea(bool _IsBattleArea,TArray<class ASpawnPoint*> SpawnPoints,
			AActor* Camera,
			class UProceduralMeshComponent* LeftMesh,
			class UProceduralMeshComponent* RightMesh,
			class UProceduralMeshComponent* NearMesh
		);


	void ExitBattleArea();

	//バトルエリアのカメラ
	AActor* BattleAreaCamera;

	TArray<class UProceduralMeshComponent*> BattleAreaMeshs;

	TArray<class ASpawnPoint*> BattleAreaSpawnPoints;

	// バトルエリアのフラグ
	bool bIsOnBattleArea;

	// バトルエリアにいる敵をスポーンする
	AActor* SpawnEnemy(ASpawnPoint* spawnPoint);

	// 
	void SpawnEnemyInBattleArea();

	// プレイヤーの削除
	void DestroyPlayer(AActor* _player);

	// プレイヤーの削除
	void RespawnPlayer(AActor* _player);


	// エネミーの削除
	void DestroyEnemy(AActor* _enemy, bool _bBattleAreaEnemy);


private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int32 BattleAreaEnemyCount = 0;

	// プレイヤーのカメラ
	AActor* PlayerCamera;

	// 
	AActor* pCamera;

	// ゲームステートのポインタ
	class ANEOGameState* pGameState;

	// プレイヤーコントローラーのポインタ
	class APlayerController* PlayerController;

	// プレイヤーのポインタ
	class APlayerBase* pPlayer;
};
