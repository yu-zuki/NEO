// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NEOGameMode.generated.h"

UCLASS()
class NEO_API ANEOGameMode : public AGameModeBase
{
	GENERATED_BODY()

	ANEOGameMode();

	// �Q�[���J�n���̏���
	void BeginPlay() override;

	// ���t���[���X�V
	void Tick(float DeltaTime) override;

public:

	UFUNCTION(BlueprintCallable, Category = "UpdateState")
		class ANEOGameState* GetGameState() { return pGameState; }

	// �v���C���[�̃J�����ύX
	void SetViewTargetWithBlend(AActor* _newViewTarget, float _blendTime = 0.f, EViewTargetBlendFunction _blendFunc = VTBlend_Linear, float _blendExp = 0.f, bool _bLockOutgoing = false);

	// ���݂̃J�����擾
	AActor* GetPlayerCamera()const { return PlayerCamera; }

	FRotator GetCameraRotation()const;

private:
	AActor* PlayerCamera;

	class ANEOGameState* pGameState;

	class APlayerController* PlayerController;

	class ACharacter* pPlayer;
};
