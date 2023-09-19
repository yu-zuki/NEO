// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NEOPlayerController.generated.h"

UCLASS()
class NEO_API ANEOPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    // �R���X�g���N�^
    ANEOPlayerController();

    // �v���C���[�̏�ԃ��Z�b�g
    void ResetPlayerStatus();

    // ���͂̃o�C���h
    virtual void SetupInputComponent() override;

    // �����ꂩ�̃L�[����͒������擾
    UFUNCTION(BlueprintCallable, Category = "SetStatus")
        bool GetIsAnyKeyPressed()const { return IsAnyKeyPressed; }

    // �v���C���[�̎c�@�擾
    int GetRemainingLives()const { return RemainingLives; }

    // �v���C���[�̎c�@���炷
    void ReduceRemainingLives() { --RemainingLives; }

    // �v���C���[������ł��邩�������Ă��炤
    void SetPlayerIsDead(bool _isDead) { PlayerIsDead = _isDead; }

    // �v���C���[������ł��邩��`����
    bool GetPlayerIsDead()const { return PlayerIsDead; }

private:

    // �S�ẴL�[�łǂ�����������Ă��邩����
    void AnyKeyPressed() { IsAnyKeyPressed = true; }

    // �S�ẴL�[�ŗ����ꂽ������
    void AnyKeyReleased() { IsAnyKeyPressed = false; }

private:

    // �v���C���[�����X�|�[���ł����
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
        int DefaultRemainingLives;

    // �v���C���[�̕����\��
    int RemainingLives;

    // �v���C���[��
    bool PlayerIsDead;

    // �L�[��������Ă��邩�ǂ���
    bool IsAnyKeyPressed;
};

