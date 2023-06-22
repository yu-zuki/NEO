// Fill out your copyright notice in the Description page of Project Settings.


#include "TGS_GameInstance.h"

UTGS_GameInstance::UTGS_GameInstance(const FObjectInitializer& ObjectInitializer)
{
	BackupGameState = EGameState::EGame_Title;	// Default
}

void UTGS_GameInstance::SaveGameStateData(EGameState& _SaveGameState)
{
	BackupGameState = _SaveGameState;			// Save
}

EGameState UTGS_GameInstance::LoadGameStateData() const
{
	return BackupGameState;						// Load	
}
