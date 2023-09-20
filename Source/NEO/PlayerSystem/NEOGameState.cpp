// Fill out your copyright notice in the Description page of Project Settings.


#include "NEOGameState.h"
#include "Kismet/GameplayStatics.h"
#include "NEOPlayerController.h"

// �R���X�g���N�^
ANEOGameState::ANEOGameState()
	: IsReadyToUpdateGame(false)
{
	// �^�C�g����Ԃɐݒ�
	GameState = EGameState_NEO::OnTitle;
}

/*
 * �֐����@�@�@�@�FInitGameState()
 * �������e�@�@�@�F�Q�[���̏�Ԃ�������
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::InitGameState()
{
	if (!PlayerController) { return; }

	// �v���C���[�̏�ԏ�����
	PlayerController->ResetPlayerStatus();
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
	// �����ꂩ�̃L�[��������Ă��邩�擾
	if (PlayerController->GetIsAnyKeyPressed() && IsReadyToUpdateGame)
	{
		// ���̃X�e�[�g��
		SetNextGameState(EGameState_NEO::OnTitle);

		// �������Ȃǂň�C�ɔ�΂Ȃ��悤�Ƀt���O�������Ă���
		IsReadyToUpdateGame = false;
	}
	else
	{
		// �^�C�g����ʂ�\��
		DisplayTitleScreen();
	}
}

/*
 * �֐����@�@�@�@�FOnOpening()
 * �������e�@�@�@�F�I�[�v�j���O�̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::OnOpening()
{

}


/*
 * �֐����@�@�@�@�FOnGamePlaying()
 * �������e�@�@�@�F�Q�[���v���C���̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::OnGamePlaying()
{
	// �Q�[���̏�ԏ�����
	InitGameState();

}

/*
 * �֐����@�@�@�@�FOnGameClear()
 * �������e�@�@�@�F�Q�[���N���A��ʂ̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::OnGameClear()
{


}


/*
 * �֐����@�@�@�@�FOnGameOver()
 * �������e�@�@�@�F�Q�[���I�[�o�[��ʂ̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::OnGameOver()
{


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