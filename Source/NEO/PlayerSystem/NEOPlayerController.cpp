// Fill out your copyright notice in the Description page of Project Settings.


#include "NEOPlayerController.h"

ANEOPlayerController::ANEOPlayerController()
    : DefaultRemainingLives(2)
    , RemainingLives(DefaultRemainingLives)
    , PlayerIsDead(false)
    , IsAnyKeyPressed(false)
{

}

// プレイヤーの状態リセット
void ANEOPlayerController::ResetPlayerStatus()
{
    // 残機設定
    RemainingLives = DefaultRemainingLives;
}


void ANEOPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // 押している間
    InputComponent->BindAction("AnyKey", IE_Pressed, this, &ANEOPlayerController::AnyKeyPressed);

    // 離した時
    InputComponent->BindAction("AnyKey", IE_Released, this, &ANEOPlayerController::AnyKeyReleased);

}
