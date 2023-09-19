// Fill out your copyright notice in the Description page of Project Settings.


#include "NEOGameState.h"
#include "Kismet/GameplayStatics.h"
#include "NEOPlayerController.h"

// �R���X�g���N�^
ANEOGameState::ANEOGameState()
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
		OnGameTitle();
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
 * �֐����@�@�@�@�FOnGameTitle()
 * �������e�@�@�@�F�^�C�g����ʂɂ���Ƃ��̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::OnGameTitle()
{
	// ���̃X�e�[�g�ֈړ�����t���O�������ĂȂ��ԃX�L�b�v
	if (!IsUpdateState) { return; }

	// �Q�[���̏�ԏ�����
	InitGameState();

	// �I�[�v�j���O��
	SetNextGameState(EGameState_NEO::OnOpening);
}

/*
 * �֐����@�@�@�@�FOnOpening()
 * �������e�@�@�@�F�^�I�[�v�j���O�̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::OnOpening()
{
	// ���̃X�e�[�g�ֈړ�����t���O�������ĂȂ��ԃX�L�b�v
	if (!IsUpdateState) { return; }

	// �C���Q�[����
	SetNextGameState(EGameState_NEO::OnGamePlaying);
}


/*
 * �֐����@�@�@�@�FOnGamePlaying()
 * �������e�@�@�@�F�Q�[���v���C���̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::OnGamePlaying()
{
	// ���̃X�e�[�g�ֈړ�����t���O�������ĂȂ��ԃX�L�b�v
	if (!IsUpdateState) { return; }

	// �v���C���[�������Ă�����N���A
	if (!PlayerController->GetPlayerIsDead())
	{
		// �N���A��
		SetNextGameState(EGameState_NEO::OnGameClear);
	}
	// ����ȊO�̓I�[�o�[
	else
	{
		// �I�[�o�[��
		SetNextGameState(EGameState_NEO::OnGameOver);
	}
	
}

/*
 * �֐����@�@�@�@�FOnGameClear()
 * �������e�@�@�@�F�Q�[���N���A��ʂ̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::OnGameClear()
{
	// ���̃X�e�[�g�ֈړ�����t���O�������ĂȂ��ԃX�L�b�v
	if (!IsUpdateState) { return; }


	// �^�C�g����
	SetNextGameState(EGameState_NEO::OnTitle);
}

/*
 * �֐����@�@�@�@�FOnGameOver()
 * �������e�@�@�@�F�Q�[���I�[�o�[��ʂ̏���
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOGameState::OnGameOver()
{
	// ���̃X�e�[�g�ֈړ�����t���O�������ĂȂ��ԃX�L�b�v
	if (!IsUpdateState) { return; }


	// �^�C�g����
	SetNextGameState(EGameState_NEO::OnTitle);
}


// ���̏�Ԃ֍X�V
void ANEOGameState::SetNextGameState(EGameState_NEO _nextGameState)
{
	// �w�肳�ꂽ�X�e�[�g�ֈړ�
	GameState = _nextGameState;

	// ����̃A�b�v�f�[�g���������Ȃ��悤�Ƀt���O�������Ă���
	IsUpdateState = false;
}