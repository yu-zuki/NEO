// Fill out your copyright notice in the Description page of Project Settings.


#include "NEOPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "NEOGameMode.h"
#include "PlayerBase.h"

ANEOPlayerController::ANEOPlayerController()
    : DefaultRemainingLives(2)
    , RemainingLives(DefaultRemainingLives)
    , PlayerIsDead(false)
{

}

// ゲーム開始時の処理
void ANEOPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // ゲームモード取得
    pGameMode = Cast<ANEOGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    // プレイヤー取得
    pPlayer = Cast<APlayerBase>(UGameplayStatics::GetPlayerCharacter(this, 0));
}


/*
 * 関数名　　　　：ResetPlayerStatus()
 * 処理内容　　　：プレイヤーの入力受付(移動処理)
 * 引数１　　　　：FInputActionValue& Value・・・入力量
 * 戻り値　　　　：なし
 */
void ANEOPlayerController::ResetPlayerStatus()
{
    // 残機設定
    RemainingLives = DefaultRemainingLives;
}


/*
 * 関数名　　　　：DestroyPlayer()
 * 処理内容　　　：プレイヤーの入力受付(移動処理)
 * 戻り値　　　　：なし
 */
void ANEOPlayerController::DestroyPlayer()
{
    if (pPlayer && pGameMode)
    {
        // プレイヤー削除
        pPlayer->Destroy();

        // プレイヤーをNULLに
        pPlayer = nullptr;

        // ゲームを次の状態へ
        pGameMode->SetNextGameState(EGameState_NEO::OnGameOver);
    }
}


/*
 * 関数名　　　　：RespawnPlayer()
 * 処理内容　　　：プレイヤーの入力受付(移動処理)
 * 引数１　　　　：FInputActionValue& Value・・・入力量
 * 戻り値　　　　：なし
 */
void ANEOPlayerController::RespawnPlayer()
{
    if (pPlayer && pGameMode)
    {
        // プレイヤーが死亡した位置取得
        FTransform RespownPos = pPlayer->GetActorTransform();

        // プレイヤーを削除
        pPlayer->Destroy();

        // プレイヤーをNULLに
        pPlayer = nullptr;

        // プレイヤーの残機を1減らす
        ReduceRemainingLives();

        // 新しいプレイヤーを生成
        pPlayer = Cast<APlayerBase>(GetWorld()->SpawnActor<APawn>(pGameMode->GetDefaultPawnClass(), RespownPos));

        // リスポーンのカメラ処理
        pGameMode->SetCameraOnPlayer();
    }
}


/*
 * 関数名　　　　：GetNowCameraRotation()
 * 処理内容　　　：カメラの回転
 * 戻り値　　　　：なし
 */
FRotator ANEOPlayerController::GetNowCameraRotation()const
{
    if (pGameMode)
    {
        // 現在のカメラを取得
        AActor* NowCamera = pGameMode->GetNowPlayerCamera();

        // 回転を取得して返す
        if (NowCamera)
        {
            // カメラのコンポーネント取得
            UCameraComponent* CameraComponent = NowCamera->FindComponentByClass<UCameraComponent>();
            if (CameraComponent)
            {
                return CameraComponent->GetComponentRotation();
            }
        }
    }

    return FRotator::ZeroRotator;
}
