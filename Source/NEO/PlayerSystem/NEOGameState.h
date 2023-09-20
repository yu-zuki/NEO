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

	// �Q�[���X�e�[�g�X�V����
	UFUNCTION(BlueprintCallable, Category = "GetParam")
		class ANEOPlayerController* GetPlayerController()const { return PlayerController; }

	// �Q�[���X�e�[�g�X�V����
	UFUNCTION(BlueprintCallable, Category = "UpdateState")
		EGameState_NEO GetGameState()const { return GameState; }

	// ���̃X�e�[�g�ֈړ����鏀�����������Ƃ��Ɏg�p
	UFUNCTION(BlueprintCallable, Category = "UpdateState")
		void SetReadyUpdateGame(bool _isReadyToUpdateGame){ IsReadyToUpdateGame = _isReadyToUpdateGame; }

	// �Q�[����C�ӂ̏�Ԃ�
	void SetNextGameState(EGameState_NEO _nextGameState);

	// �^�C�g����ʂ�\��
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Title")
		void DisplayTitleScreen();

	virtual void DisplayTitleScreen_Implementation() {};

	// �f����ʂ�\��
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "DEMO")
		void DisplayDemoScreen();

	virtual void DisplayDemoScreen_Implementation() {};

private:

	// �Q�[���̏�Ԃ�������
	void InitGameState();							

	// �^�C�g���̏���
	void OnTitle();

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

	// �Q�[�������̏�ԂɃA�b�v�f�[�g���鏀�����ł�����
	bool IsReadyToUpdateGame;

	// �v���C���[�̃R���g���[���[�i�[�p
	class ANEOPlayerController* PlayerController;
};
