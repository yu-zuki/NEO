// Fill out your copyright notice in the Description page of Project Settings.


#include "NEOPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "NEOGameMode.h"
#include "PlayerBase.h"

ANEOPlayerController::ANEOPlayerController()
    : DefaultRemainingLives(2)
    , RemainingLives(DefaultRemainingLives)
    , PlayerIsDead(false)
{

}

// �Q�[���J�n���̏���
void ANEOPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // �Q�[�����[�h�擾
    pGameMode = Cast<ANEOGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    // �v���C���[�擾
    pPlayer = Cast<APlayerBase>(UGameplayStatics::GetPlayerCharacter(this, 0));
}


/*
 * �֐����@�@�@�@�FResetPlayerStatus()
 * �������e�@�@�@�F�v���C���[�̓��͎�t(�ړ�����)
 * �����P�@�@�@�@�FFInputActionValue& Value�E�E�E���͗�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOPlayerController::ResetPlayerStatus()
{
    // �c�@�ݒ�
    RemainingLives = DefaultRemainingLives;
}


/*
 * �֐����@�@�@�@�FDestroyPlayer()
 * �������e�@�@�@�F�v���C���[�̓��͎�t(�ړ�����)
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOPlayerController::DestroyPlayer()
{
    if (pPlayer && pGameMode)
    {
        // �v���C���[�폜
        pPlayer->Destroy();

        // �v���C���[��NULL��
        pPlayer = nullptr;

        // �Q�[�������̏�Ԃ�
        pGameMode->SetNextGameState(EGameState_NEO::OnGameOver);
    }
}


/*
 * �֐����@�@�@�@�FRespawnPlayer()
 * �������e�@�@�@�F�v���C���[�̓��͎�t(�ړ�����)
 * �����P�@�@�@�@�FFInputActionValue& Value�E�E�E���͗�
 * �߂�l�@�@�@�@�F�Ȃ�
 */
void ANEOPlayerController::RespawnPlayer()
{
    if (pPlayer && pGameMode)
    {
        // �v���C���[�����S�����ʒu�擾
        FTransform RespownPos = pPlayer->GetActorTransform();

        // �v���C���[���폜
        pPlayer->Destroy();

        // �v���C���[��NULL��
        pPlayer = nullptr;

        // �v���C���[�̎c�@��1���炷
        ReduceRemainingLives();

        // �V�����v���C���[�𐶐�
        pPlayer = Cast<APlayerBase>(GetWorld()->SpawnActor<APawn>(pGameMode->GetDefaultPawnClass(), RespownPos));

        // ���X�|�[���̃J��������
        pGameMode->SetCameraOnPlayer();
    }
}


/*
 * �֐����@�@�@�@�FGetNowCameraRotation()
 * �������e�@�@�@�F�J�����̉�]
 * �߂�l�@�@�@�@�F�Ȃ�
 */
FRotator ANEOPlayerController::GetNowCameraRotation()const
{
    if (pGameMode)
    {
        // ���݂̃J�������擾
        AActor* NowCamera = pGameMode->GetNowPlayerCamera();

        // ��]���擾���ĕԂ�
        if (NowCamera)
        {
            // �J�����̃R���|�[�l���g�擾
            UCameraComponent* CameraComponent = NowCamera->FindComponentByClass<UCameraComponent>();
            if (CameraComponent)
            {
                return CameraComponent->GetComponentRotation();
            }
        }
    }

    return FRotator::ZeroRotator;
}
