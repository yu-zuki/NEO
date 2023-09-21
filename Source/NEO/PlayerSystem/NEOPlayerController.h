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

    // �Q�[���J�n���̏���
    void BeginPlay();

    // �����ꂩ�̃L�[����͒������擾
    UFUNCTION(BlueprintCallable, Category = "SetStatus")
        bool GetIsAnyKeyPressed()const { return IsInputKeyDown("AnyKey"); }

    // �v���C���[�̏�ԃ��Z�b�g
    void ResetPlayerStatus();

    // �v���C���[�̎c�@�擾
    int GetRemainingLives()const { return RemainingLives; }

    // �v���C���[�̎c�@���炷
    void ReduceRemainingLives() { --RemainingLives; }

    // �v���C���[�̍폜5rttrt5r
    void DestroyPlayer();

    // �v���C���[�̃��X�|�[��
    void RespawnPlayer();

    // ���݂̃v���C���[�̃J�����擾
    FRotator GetNowCameraRotation()const;

    // �v���C���[������ł��邩��`����
    bool GetPlayerIsDead()const { return PlayerIsDead; }

private:

    // �v���C���[�����X�|�[���ł����
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
        int DefaultRemainingLives;

    // �v���C���[�̕����\��
    int RemainingLives;

    // �v���C���[��
    bool PlayerIsDead;

    // �v���C���[�̃|�C���^
    class APlayerBase* pPlayer;

    // �Q�[�����[�h
    class ANEOGameMode* pGameMode;
};

