// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TGS_GameStateBase.h"

#include "TGS_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class NEO_API UTGS_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
//�R���X�g���N�^
	UTGS_GameInstance(const FObjectInitializer& ObjectInitializer);

protected:
	//�ۑ��̃f�[�^
	EGameState BackupGameState;

	//PlayerType�̏�Ԃ�ۑ�
	EPlayerType BackupPlayerType;

	////Player�̏�Ԃ�ۑ�
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameState")
	//EPlayerState BackupPlayerState;

public:

	//�Q�[���̏�Ԃ�ۑ�
	UFUNCTION(BlueprintCallable, Category = "GameState")
	void SaveGameStateData(EGameState& _SaveGameState);

	//�Q�[���̏�Ԃ��擾
	UFUNCTION(BlueprintCallable, Category = "GameState")
	EGameState LoadGameStateData() const;

	//�v���C���[�̏�Ԃ�ۑ�
	UFUNCTION(BlueprintCallable, Category = "GameState")
	void SavePlayerType(EPlayerType& _SavePlayerType);

	//�v���C���[�̏�Ԃ��擾
	UFUNCTION(BlueprintCallable, Category = "GameState")
	EPlayerType LoadPlayerType() const;

};
