// Fill out your copyright notice in the Description page of Project Settings.


#include "NEOGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "NEOGameState.h"

ANEOGameMode::ANEOGameMode()
{

}


void ANEOGameMode::BeginPlay()
{
	Super::BeginPlay();

	// ゲームステートを取得
	pGameState = Cast<ANEOGameState>(UGameplayStatics::GetGameState(GetWorld()));
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