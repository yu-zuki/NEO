// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NEOGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NEO_API ANEOGameMode : public AGameModeBase
{
	GENERATED_BODY()

	ANEOGameMode();

public:

	UFUNCTION(BlueprintCallable, Category = "UpdateState")
		class ANEOGameState* GetGameState() { return pGameState; }
	

protected:

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

private:

	class ANEOGameState* pGameState;
};
