// Fill out your copyright notice in the Description page of Project Settings.


#include "NEOGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "NEOGameState.h"
#include "Camera/CameraComponent.h"

ANEOGameMode::ANEOGameMode()
{

}


void ANEOGameMode::BeginPlay()
{
	Super::BeginPlay();

	// �Q�[���X�e�[�g���擾
	pGameState = Cast<ANEOGameState>(UGameplayStatics::GetGameState(GetWorld()));

	// �v���C���[�R���g���[���[���擾
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void ANEOGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// �Q�[���̏�Ԃ��X�V
	if (pGameState) 
	{
		// �Q�[���̏�ԍX�V
		pGameState->UpdateGameState(DeltaTime);

	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Game State is not found"));
		pGameState = Cast<ANEOGameState>(UGameplayStatics::GetGameState(GetWorld()));
	}
}

void ANEOGameMode::SetViewTargetWithBlend(AActor* _newViewTarget, float _blendTime, EViewTargetBlendFunction _blendFunc, float _blendExp, bool _bLockOutgoing)
{
	// �R���g���[���[�����鎞�J�������u�����h
	if (PlayerController)
	{
		PlayerController->SetViewTargetWithBlend(_newViewTarget, _blendTime);

		// �v���C���[�̃J�������u�����h��ɐݒ�
		PlayerCamera = _newViewTarget;
	}
}

FRotator ANEOGameMode::GetCameraRotation()const
{
	if (PlayerCamera)
	{
		// �J�����̃R���|�[�l���g�擾
		UCameraComponent* CameraComponent = PlayerCamera->FindComponentByClass<UCameraComponent>();
		if (CameraComponent) 
		{
			return CameraComponent->GetComponentRotation();
		}
	}

	return FRotator::ZeroRotator;
}
