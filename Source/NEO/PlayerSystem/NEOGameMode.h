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

	// 現在のカメラ取得
	AActor* GetPlayerCamera()const { return PlayerCamera; }

	FRotator GetCameraRotation()const;

private:
	AActor* PlayerCamera;

	class ANEOGameState* pGameState;

	class APlayerController* PlayerController;

	class ACharacter* pPlayer;
};
