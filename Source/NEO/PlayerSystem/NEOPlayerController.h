// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NEOPlayerController.generated.h"

UCLASS()
class NEO_API ANEOPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    // コンストラクタ
    ANEOPlayerController();

    // ゲーム開始時の処理
    void BeginPlay();

    // いずれかのキーを入力中かを取得
    UFUNCTION(BlueprintCallable, Category = "SetStatus")
        bool GetIsAnyKeyPressed()const { return IsInputKeyDown("AnyKey"); }

    // プレイヤーの状態リセット
    void ResetPlayerStatus();

    // プレイヤーの残機取得
    int GetRemainingLives()const { return RemainingLives; }

    // プレイヤーの残機減らす
    void ReduceRemainingLives() { --RemainingLives; }

    // プレイヤーの削除5rttrt5r
    void DestroyPlayer();

    // プレイヤーのリスポーン
    void RespawnPlayer();

    // 現在のプレイヤーのカメラ取得
    FRotator GetNowCameraRotation()const;

    // プレイヤーが死んでいるかを伝える
    bool GetPlayerIsDead()const { return PlayerIsDead; }

private:

    // プレイヤーがリスポーンできる回数
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
        int DefaultRemainingLives;

    // プレイヤーの復活可能回数
    int RemainingLives;

    // プレイヤーが
    bool PlayerIsDead;

    // プレイヤーのポインタ
    class APlayerBase* pPlayer;

    // ゲームモード
    class ANEOGameMode* pGameMode;
};

