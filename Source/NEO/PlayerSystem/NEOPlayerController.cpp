// Fill out your copyright notice in the Description page of Project Settings.


#include "NEOPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "NEOGameMode.h"
#include "PlayerBase.h"

ANEOPlayerController::ANEOPlayerController()
    : DefaultRemainingLives(2)
    , EnemiesCnt(3)
    , PlayerToEnemyDistance(200.f)
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

        // �v���C���[�����S��Ԃ�
        PlayerIsDead = true;

        // �Q�[�������̏�Ԃ�
        pGameMode->SetNextGameState();
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


/*
 * �֐����@�@�@�@�FGetIsDebugKeyPressed()
 * �������e�@�@�@�F�f�o�b�O�L�[�ɐݒ肳��Ă���L�[��������Ă��邩
 * �߂�l�@�@�@�@�F�Ȃ�
 */
bool ANEOPlayerController::GetIsDebugKeyPressed()const
{
    for (int i = 0; i < sizeof(KeyNames) / sizeof(FName); ++i)
    {
        if (IsInputKeyDown(KeyNames[i]))
        {
            return false;
        }

    }

    return true;
}


bool ANEOPlayerController::AnyEnemiesNearbyMoreSecond()const
{
    if (pGameMode)
    {
        // �Q�[�����[�h���猻�ݏo�Ă���G�̏����擾
        TArray<AActor*> Enemies = pGameMode->GetEnemies();

        // �v���C���[�̈ʒu�擾
        FVector PlayerPos = pPlayer->GetActorLocation();

        // �߂��ɓG���������̃J�E���g
        int NearByEnemiesCnt = 0;
        // �v���C���[�ƓG�Ƃ̋������v��
        for (int i = 0; i < Enemies.Num(); ++i)
        {
            if (Enemies[i])
            {
                // �G�̈ʒu�擾
                FVector EnemyPos = Enemies[i]->GetActorLocation();
                // �v���C���[�Ƌ����v�Z
                float Distance = FVector::Dist(EnemyPos, PlayerPos);

                if (Distance <= PlayerToEnemyDistance)
                {
                    ++NearByEnemiesCnt;
                }
            }

            if (NearByEnemiesCnt >= EnemiesCnt)
            {
                return true;
            }
        }
    }

    return false;
}