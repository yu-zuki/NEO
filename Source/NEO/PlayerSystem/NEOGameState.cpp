// Fill out your copyright notice in the Description page of Project Settings.


#include "NEOGameState.h"
#include "Kismet/GameplayStatics.h"
#include "NEOPlayerController.h"

// コンストラクタ
ANEOGameState::ANEOGameState()
{
	// タイトル状態に設定
	GameState = EGameState_NEO::OnTitle;
}

/*
 * 関数名　　　　：InitGameState()
 * 処理内容　　　：ゲームの状態を初期化
 * 戻り値　　　　：なし
 */
void ANEOGameState::InitGameState()
{
	if (!PlayerController) { return; }

	// プレイヤーの状態初期化
	PlayerController->ResetPlayerStatus();
}

/*
 * 関数名　　　　：UpdateGameState()
 * 処理内容　　　：ゲームの状態を更新
 * 引数１　　　　：float DeltaTime・・・毎フレームかかった時間
 * 戻り値　　　　：なし
 */
void ANEOGameState::UpdateGameState(float DeltaTime)
{
	switch (GameState)
	{
	case EGameState_NEO::OnTitle:
		OnGameTitle();
		break;
	case EGameState_NEO::OnOpening:
		OnOpening();
		break;
	case EGameState_NEO::OnGamePlaying:
		OnGamePlaying();
		break;
	case EGameState_NEO::OnGameClear:
		OnGameClear();
		break;
	case EGameState_NEO::OnGameOver:
		OnGameOver();
		break;
	default:

		break;

	}
}


/*
 * 関数名　　　　：OnGameTitle()
 * 処理内容　　　：タイトル画面にいるときの処理
 * 戻り値　　　　：なし
 */
void ANEOGameState::OnGameTitle()
{
	// ゲームの状態初期化
	InitGameState();

}

/*
 * 関数名　　　　：OnOpening()
 * 処理内容　　　：オープニングの処理
 * 戻り値　　　　：なし
 */
void ANEOGameState::OnOpening()
{

}


/*
 * 関数名　　　　：OnGamePlaying()
 * 処理内容　　　：ゲームプレイ中の処理
 * 戻り値　　　　：なし
 */
void ANEOGameState::OnGamePlaying()
{

}

/*
 * 関数名　　　　：OnGameClear()
 * 処理内容　　　：ゲームクリア画面の処理
 * 戻り値　　　　：なし
 */
void ANEOGameState::OnGameClear()
{


}


/*
 * 関数名　　　　：OnGameOver()
 * 処理内容　　　：ゲームオーバー画面の処理
 * 戻り値　　　　：なし
 */
void ANEOGameState::OnGameOver()
{


}


/*
 * 関数名　　　　：SetNextGameState()
 * 処理内容　　　：ゲームを指定された状態へ
 * 戻り値　　　　：なし
 */
void ANEOGameState::SetNextGameState(EGameState_NEO _nextGameState)
{
	// 指定されたステートへ移動
	GameState = _nextGameState;

}