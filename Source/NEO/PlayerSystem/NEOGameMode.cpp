// Fill out your copyright notice in the Description page of Project Settings.


#include "NEOGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "NEOGameState.h"
#include "Camera/CameraComponent.h"

ANEOGameMode::ANEOGameMode()
{

}


void ANEOGameMode::BeginPlay()
{
	Super::BeginPlay();

	// ゲームステートを取得
	pGameState = Cast<ANEOGameState>(UGameplayStatics::GetGameState(GetWorld()));

	// プレイヤーコントローラーを取得
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void ANEOGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// ゲームの状態を更新
	if (pGameState) 
	{
		// ゲームの状態更新
		pGameState->UpdateGameState(DeltaTime);

	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Game State is not found"));
		pGameState = Cast<ANEOGameState>(UGameplayStatics::GetGameState(GetWorld()));
	}
}

void ANEOGameMode::SetViewTargetWithBlend(AActor* _newViewTarget, float _blendTime, EViewTargetBlendFunction _blendFunc, float _blendExp, bool _bLockOutgoing)
{
	// コントローラーがある時カメラをブレンド
	if (PlayerController)
	{
		PlayerController->SetViewTargetWithBlend(_newViewTarget, _blendTime);

		// プレイヤーのカメラをブレンド先に設定
		PlayerCamera = _newViewTarget;
	}
}

FRotator ANEOGameMode::GetCameraRotation()const
{
	if (PlayerCamera)
	{
		// カメラのコンポーネント取得
		UCameraComponent* CameraComponent = PlayerCamera->FindComponentByClass<UCameraComponent>();
		if (CameraComponent) 
		{
			return CameraComponent->GetComponentRotation();
		}
	}

	return FRotator::ZeroRotator;
}
