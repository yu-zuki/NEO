// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NEOGameState.generated.h"


// ゲームの状態を管理するEnum
UENUM(BlueprintType)
enum class EGameState_NEO : uint8
{
	OnTitle			UMETA(DisplayName = "Title"),			// タイトル画面
	OnOpening		UMETA(DisplayName = "Opening"),			// オープニング
	OnGamePlaying	UMETA(DisplayName = "InGame"),			// インゲーム
	OnGameClear		UMETA(DisplayName = "GameClear"),		// クリア
	OnGameOver		UMETA(DisplayName = "GameOver")			// オーバー
};


UCLASS()
class NEO_API ANEOGameState : public AGameStateBase
{
	GENERATED_BODY()


	// コンストラクタ
	ANEOGameState();

public:

	// 次の状態へいく時に使用
	UFUNCTION(BlueprintCallable, Category = "UpdateState")
		void UpdateState() { IsUpdateState = true; }

	// ゲームの状態を更新
	void UpdateGameState(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "UpdateState")
		EGameState_NEO GetGameState() { return GameState; }

private:

	// ゲームの状態を初期化
	void InitGameState();							


	// タイトルの処理
	void OnGameTitle();

	// オープニングの処理
	void OnOpening();

	// インゲームの処理
	void OnGamePlaying();

	// クリアの処理
	void OnGameClear();

	// オーバーの処理
	void OnGameOver();	

	// 次の状態へ更新
	void SetNextGameState(EGameState_NEO _nextGameState);

private:

	// ゲームの状態を管理
	EGameState_NEO GameState;

	// 次の状態へ更新するかどうか
	bool IsUpdateState;

	// プレイヤーのコントローラー格納用
	class ANEOPlayerController* PlayerController;
};
