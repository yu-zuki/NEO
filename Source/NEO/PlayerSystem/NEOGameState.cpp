// Fill out your copyright notice in the Description page of Project Settings.


#include "NEOGameState.h"
#include "Kismet/GameplayStatics.h"
#include "NEOPlayerController.h"
#include "NEOGameMode.h"

// �R���X�g���N�^
ANEOGameState::ANEOGameState()
	: IsReadyToUpdateGame(false)
{
}

/*
 * �֐����@�@�@�@�FInitGameState()
 * �������e�@�@�@�F�Q�[���̏�Ԃ�������
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::InitGameState()
{
	if (PlayerController) { return; }

	// �^�C�g����Ԃɐݒ�
	GameState = EGameState_NEO::OnTitle;

	// �R���g���[���[�擾
	PlayerController = Cast<ANEOPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// �v���C���[�̏�ԏ�����
	PlayerController->ResetPlayerStatus();
}


/*
 * �֐����@�@�@�@�FInitInGame()
 * �������e�@�@�@�F�C���Q�[���̏�Ԃ��X�V
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::InitInGame()
{
	// �Q�[�����[�h�擾
	ANEOGameMode* pGameMode = Cast<ANEOGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (pGameMode)
	{
		// �v���C���[�ɃJ������ݒ�
		pGameMode->InitCameraOnPlayer();
	}
}

/*
 * �֐����@�@�@�@�FResetGame()
 * �������e�@�@�@�F�C���Q�[���̏�Ԃ��X�V
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::RestartGame()
{
	// �Q�[�����[�h�擾
	ANEOGameMode* pGameMode = Cast<ANEOGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (pGameMode)
	{
		// �v���C���[�ɃJ������ݒ�
		pGameMode->RestartGame();
	}
}


/*
 * �֐����@�@�@�@�FUpdateGameState()
 * �������e�@�@�@�F�Q�[���̏�Ԃ��X�V
 * �����P�@�@�@�@�Ffloat DeltaTime�E�E�E���t���[��������������
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::UpdateGameState(float DeltaTime)
{
	switch (GameState)
	{
	case EGameState_NEO::OnTitle:
		OnTitle();
		break;
	case EGameState_NEO::OnOpening:
		OnOpening();
		break;
	case EGameState_NEO::OnGamePlaying:
		OnGamePlaying();
		break;
	case EGameState_NEO::OnGameClear:
		OnGameClear();
		break;
	case EGameState_NEO::OnGameOver:
		OnGameOver();
		break;
	default:

		break;

	}

}


/*
 * �֐����@�@�@�@�FOnTitle()
 * �������e�@�@�@�F�^�C�g����ʂɂ���Ƃ��̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::OnTitle()
{
	// �Q�[���̏�ԏ�����
	InitGameState();

	if (!PlayerController) { return; }

	// �^�C�g����ʂ��\������Ă���Ƃ����ɐi�߂�
	IsReadyToUpdateGame = (TitleState == ETitleState_NEO::OnTitleDisplay);

	// �����ꂩ�̃L�[��������Ă��邩�擾
	if (IsReadyToUpdateGame && PlayerController->GetIsAnyKeyPressed())
	{
		// ���̃X�e�[�g��
		SetNextGameState(EGameState_NEO::OnOpening);

		// �\������Ă���Widget�͏���
		DeleteWidget();

		// �^�C�g����Ԃɐݒ�
		TitleState = ETitleState_NEO::OnDisplay_None;

		// �������Ȃǂň�C�ɔ�΂Ȃ��悤�Ƀt���O�������Ă���
		IsReadyToUpdateGame = false;
	}
}

/*
 * �֐����@�@�@�@�FOnOpening()
 * �������e�@�@�@�F�I�[�v�j���O�̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::OnOpening()
{
	if (!PlayerController) { return; }

	// BP���ŃI�[�v�j���O�Đ��I���Ȃǂ��擾
	if (IsReadyToUpdateGame)
	{
		// ���̃X�e�[�g��
		SetNextGameState(EGameState_NEO::OnGamePlaying);

		// �t���O�������Ă���
		IsReadyToUpdateGame = false;

		// �C���Q�[��������
		InitInGame();
	}
}


/*
 * �֐����@�@�@�@�FOnGamePlaying()
 * �������e�@�@�@�F�Q�[���v���C���̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::OnGamePlaying()
{
	if (!PlayerController) { return; }

	if (IsReadyToUpdateGame)
	{
		// �v���C���[������ł�����Q�[���I�[�o�[
		if (PlayerController->GetPlayerIsDead())
		{
			// ���̃X�e�[�g��
			SetNextGameState(EGameState_NEO::OnGameOver);
		}
		// ����ȊO�̓Q�[���N���A
		else
		{
			// ���̃X�e�[�g��
			SetNextGameState(EGameState_NEO::OnGameClear);
		}

		// �t���O�������Ă���
		IsReadyToUpdateGame = false;
	}
}

/*
 * �֐����@�@�@�@�FOnGameClear()
 * �������e�@�@�@�F�Q�[���N���A��ʂ̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::OnGameClear()
{
	if (IsReadyToUpdateGame)
	{
		// ���̃X�e�[�g��
		SetNextGameState(EGameState_NEO::OnTitle);

		// �Q�[�����Z�b�g
		RestartGame();
	}	
}


/*
 * �֐����@�@�@�@�FOnGameOver()
 * �������e�@�@�@�F�Q�[���I�[�o�[��ʂ̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::OnGameOver()
{
	if (IsReadyToUpdateGame)
	{
		// ���̃X�e�[�g��
		SetNextGameState(EGameState_NEO::OnTitle);

		// �Q�[�����Z�b�g
		RestartGame();
	}
}


/*
 * �֐����@�@�@�@�FSetNextGameState()
 * �������e�@�@�@�F�Q�[�����w�肳�ꂽ��Ԃ�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::SetNextGameState(EGameState_NEO _nextGameState)
{
	// �w�肳�ꂽ�X�e�[�g�ֈړ�
	GameState = _nextGameState;

}