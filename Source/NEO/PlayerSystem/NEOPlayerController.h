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
        bool GetIsAnyKeyPressed()const { return IsInputKeyDown("AnyKey") && GetIsDebugKeyPressed(); }

    UFUNCTION(BlueprintCallable, Category = "SetStatus")
        bool AnyEnemiesNearbyMoreSecond()const;

    // �v���C���[�̏�ԃ��Z�b�g
    void ResetPlayerStatus();

    // �v���C���[�̎c�@�擾
    int GetRemainingLives()const { return RemainingLives; }

    // �v���C���[�̎c�@���炷
    void ReduceRemainingLives() { --RemainingLives; }

    // �v���C���[�̍폜
    void DestroyPlayer();

    // �v���C���[�̃��X�|�[��
    void RespawnPlayer();

    // ���݂̃v���C���[�̃J�����擾
    FRotator GetNowCameraRotation()const;

    // �v���C���[������ł��邩��`����
    bool GetPlayerIsDead()const { return PlayerIsDead; }

private:

    bool GetIsDebugKeyPressed()const;



private:

    // �v���C���[�����X�|�[���ł����
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
        int DefaultRemainingLives;

    // �v���C���[�̋߂��ɉ��̓G�������狭���\�����邩
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
        int EnemiesCnt;

    // �v���C���[�����X�|�[���ł����
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
        float PlayerToEnemyDistance;

    FName KeyNames[10] = { "F1","F2","F3","F4","F5","F6","F7","F8","F9","F10" };

    // �v���C���[�̕����\��
    int RemainingLives;

    // �v���C���[��
    bool PlayerIsDead;

    // �v���C���[�̃|�C���^
    class APlayerBase* pPlayer;

    // �Q�[�����[�h
    class ANEOGameMode* pGameMode;
};

