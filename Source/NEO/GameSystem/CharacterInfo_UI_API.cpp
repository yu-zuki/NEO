// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterInfo_UI_API.h"
#include "TGS_GameMode.h"

/**
 * �L�����N�^�[�����擾����
 *
 * @return	�L�����N�^�[���̃^�v���B������Ȃ������ꍇ�� Not Found�A0�A0 �̃^�v����Ԃ��B
 */
TTuple<FName, int, int> UCharacterInfo_UI_API::GetCharacterInfo()
{
	return TTuple<FName, int, int>("Not Found", 0, 0);
}

/**
 * UI ��G�L�����N�^�[�p�ɐݒ肷��
 *
 * @remarks	ATGS_GameMode ��������Ȃ������ꍇ�� Warning ���O���o�͂���B
 */
void UCharacterInfo_UI_API::SetUI_Enemy()
{
	ATGS_GameMode* GameMode = Cast<ATGS_GameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		TTuple<FName, int, int> CharacterInfo = GetCharacterInfo();

		GameMode->SetUI_Enemy(CharacterInfo.Get<0>(), CharacterInfo.Get<1>(), CharacterInfo.Get<2>());
	}
	else
	{
		UE_LOG( LogTemp, Warning, TEXT("GameMode is not found") );
	}
}
