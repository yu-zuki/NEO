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
	// 次のステートへ移動するフラグが立ってない間スキップ
	if (!IsUpdateState) { return; }

	// ゲームの状態初期化
	InitGameState();

	// オープニングへ
	SetNextGameState(EGameState_NEO::OnOpening);
}

/*
 * 関数名　　　　：OnOpening()
 * 処理内容　　　：タオープニングの処理
 * 戻り値　　　　：なし
 */
void ANEOGameState::OnOpening()
{
	// 次のステートへ移動するフラグが立ってない間スキップ
	if (!IsUpdateState) { return; }

	// インゲームへ
	SetNextGameState(EGameState_NEO::OnGamePlaying);
}


/*
 * 関数名　　　　：OnGamePlaying()
 * 処理内容　　　：ゲームプレイ中の処理
 * 戻り値　　　　：なし
 */
void ANEOGameState::OnGamePlaying()
{
	// 次のステートへ移動するフラグが立ってない間スキップ
	if (!IsUpdateState) { return; }

	// プレイヤーが生きていたらクリア
	if (!PlayerController->GetPlayerIsDead())
	{
		// クリアへ
		SetNextGameState(EGameState_NEO::OnGameClear);
	}
	// それ以外はオーバー
	else
	{
		// オーバーへ
		SetNextGameState(EGameState_NEO::OnGameOver);
	}
	
}

/*
 * 関数名　　　　：OnGameClear()
 * 処理内容　　　：ゲームクリア画面の処理
 * 戻り値　　　　：なし
 */
void ANEOGameState::OnGameClear()
{
	// 次のステートへ移動するフラグが立ってない間スキップ
	if (!IsUpdateState) { return; }


	// タイトルへ
	SetNextGameState(EGameState_NEO::OnTitle);
}

/*
 * 関数名　　　　：OnGameOver()
 * 処理内容　　　：ゲームオーバー画面の処理
 * 戻り値　　　　：なし
 */
void ANEOGameState::OnGameOver()
{
	// 次のステートへ移動するフラグが立ってない間スキップ
	if (!IsUpdateState) { return; }


	// タイトルへ
	SetNextGameState(EGameState_NEO::OnTitle);
}


// 次の状態へ更新
void ANEOGameState::SetNextGameState(EGameState_NEO _nextGameState)
{
	// 指定されたステートへ移動
	GameState = _nextGameState;

	// 次回のアップデートが発動しないようにフラグを下げておく
	IsUpdateState = false;
}