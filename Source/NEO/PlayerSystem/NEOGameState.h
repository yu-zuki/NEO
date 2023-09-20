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

	// ゲームの状態を更新
	void UpdateGameState(float DeltaTime);

	// ゲームステート更新処理
	UFUNCTION(BlueprintCallable, Category = "GetParam")
		class ANEOPlayerController* GetPlayerController()const { return PlayerController; }

	// ゲームステート更新処理
	UFUNCTION(BlueprintCallable, Category = "UpdateState")
		EGameState_NEO GetGameState()const { return GameState; }

	// 次のステートへ移動する準備が整ったときに使用
	UFUNCTION(BlueprintCallable, Category = "UpdateState")
		void SetReadyUpdateGame(bool _isReadyToUpdateGame){ IsReadyToUpdateGame = _isReadyToUpdateGame; }

	// ゲームを任意の状態へ
	void SetNextGameState(EGameState_NEO _nextGameState);

	// タイトル画面を表示
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Title")
		void DisplayTitleScreen();

	virtual void DisplayTitleScreen_Implementation() {};

	// デモ画面を表示
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "DEMO")
		void DisplayDemoScreen();

	virtual void DisplayDemoScreen_Implementation() {};

private:

	// ゲームの状態を初期化
	void InitGameState();							

	// タイトルの処理
	void OnTitle();

	// オープニングの処理
	void OnOpening();

	// インゲームの処理
	void OnGamePlaying();

	// クリアの処理
	void OnGameClear();

	// オーバーの処理
	void OnGameOver();	

private:

	// ゲームの状態を管理
	EGameState_NEO GameState;

	// ゲームを次の状態にアップデートする準備ができたか
	bool IsReadyToUpdateGame;

	// プレイヤーのコントローラー格納用
	class ANEOPlayerController* PlayerController;
};
