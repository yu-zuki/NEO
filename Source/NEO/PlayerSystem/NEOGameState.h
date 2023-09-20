// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NEOGameState.generated.h"


// �Q�[���̏�Ԃ��Ǘ�����Enum
UENUM(BlueprintType)
enum class EGameState_NEO : uint8
{
	OnTitle			UMETA(DisplayName = "Title"),			// �^�C�g�����
	OnOpening		UMETA(DisplayName = "Opening"),			// �I�[�v�j���O
	OnGamePlaying	UMETA(DisplayName = "InGame"),			// �C���Q�[��
	OnGameClear		UMETA(DisplayName = "GameClear"),		// �N���A
	OnGameOver		UMETA(DisplayName = "GameOver")			// �I�[�o�[
};


UCLASS()
class NEO_API ANEOGameState : public AGameStateBase
{
	GENERATED_BODY()


	// �R���X�g���N�^
	ANEOGameState();

public:

	// �Q�[���̏�Ԃ��X�V
	void UpdateGameState(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "UpdateState")
		EGameState_NEO GetGameState()const { return GameState; }

	// �Q�[����C�ӂ̏�Ԃ�
	void SetNextGameState(EGameState_NEO _nextGameState);

private:

	// �Q�[���̏�Ԃ�������
	void InitGameState();							


	// �^�C�g���̏���
	void OnGameTitle();

	// �I�[�v�j���O�̏���
	void OnOpening();

	// �C���Q�[���̏���
	void OnGamePlaying();

	// �N���A�̏���
	void OnGameClear();

	// �I�[�o�[�̏���
	void OnGameOver();	

private:

	// �Q�[���̏�Ԃ��Ǘ�
	EGameState_NEO GameState;

	// �v���C���[�̃R���g���[���[�i�[�p
	class ANEOPlayerController* PlayerController;
};
