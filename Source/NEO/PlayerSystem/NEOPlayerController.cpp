// Fill out your copyright notice in the Description page of Project Settings.


#include "NEOPlayerController.h"

ANEOPlayerController::ANEOPlayerController()
    : DefaultRemainingLives(2)
    , RemainingLives(DefaultRemainingLives)
    , PlayerIsDead(false)
    , IsAnyKeyPressed(false)
{

}

// �v���C���[�̏�ԃ��Z�b�g
void ANEOPlayerController::ResetPlayerStatus()
{
    // �c�@�ݒ�
    RemainingLives = DefaultRemainingLives;
}


void ANEOPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // �����Ă����
    InputComponent->BindAction("AnyKey", IE_Pressed, this, &ANEOPlayerController::AnyKeyPressed);

    // ��������
    InputComponent->BindAction("AnyKey", IE_Released, this, &ANEOPlayerController::AnyKeyReleased);

}
