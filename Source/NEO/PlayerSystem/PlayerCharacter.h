// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBase.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class NEO_API APlayerCharacter : public APlayerBase
{
	GENERATED_BODY()


private:

	// �R���X�g���N�^
	APlayerCharacter();

	// �Q�[���J�n���ɌĂяo����鏈��
	virtual void BeginPlay() override;

	// ���t���[���Ăяo����鏈��
	virtual void Tick(float DeltaTime) override;

	// ���͂̃Z�b�g�A�b�v
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �v���C���[�̃f�[�^��������
	virtual void SetupPlayerData() override;

	// �A�j���[�V�����A�Z�b�g��ݒ�
	void SetupAnimationAsset();

public:

	// AnimNotify�ŌĂяo��
	// �R���W��������
	void SetCollision();

};
