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

    // プレイヤーの状態リセット
    void ResetPlayerStatus();

    // 入力のバインド
    virtual void SetupInputComponent() override;

    // いずれかのキーを入力中かを取得
    UFUNCTION(BlueprintCallable, Category = "SetStatus")
        bool GetIsAnyKeyPressed()const { return IsAnyKeyPressed; }

    // プレイヤーの残機取得
    int GetRemainingLives()const { return RemainingLives; }

    // プレイヤーの残機減らす
    void ReduceRemainingLives() { --RemainingLives; }

    // プレイヤーが死んでいるかを教えてもらう
    void SetPlayerIsDead(bool _isDead) { PlayerIsDead = _isDead; }

    // プレイヤーが死んでいるかを伝える
    bool GetPlayerIsDead()const { return PlayerIsDead; }

private:

    // 全てのキーでどこかが押されているか判定
    void AnyKeyPressed() { IsAnyKeyPressed = true; }

    // 全てのキーで離されたか判定
    void AnyKeyReleased() { IsAnyKeyPressed = false; }

private:

    // プレイヤーがリスポーンできる回数
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
        int DefaultRemainingLives;

    // プレイヤーの復活可能回数
    int RemainingLives;

    // プレイヤーが
    bool PlayerIsDead;

    // キーが押されているかどうか
    bool IsAnyKeyPressed;
};

