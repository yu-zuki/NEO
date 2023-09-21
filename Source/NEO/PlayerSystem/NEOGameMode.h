// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NEOGameState.h"
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
		class ANEOGameState* GetGameState()const { return pGameState; }

	// �v���C���[�̃J�����ύX
	void SetViewTargetWithBlend(AActor* _newViewTarget, float _blendTime = 0.f, EViewTargetBlendFunction _blendFunc = VTBlend_Linear, float _blendExp = 0.f, bool _bLockOutgoing = false);

	// �v���C���[�̃J�����ݒ�
	void SetPlayerCamera(AActor* _playerCamera) { PlayerCamera = _playerCamera; }

	void InitCameraOnPlayer();
	
	// ���݂̃J�����擾
	AActor* GetNowPlayerCamera()const;

	UFUNCTION(BlueprintCallable, Category = "Area")
		void SetIsOnBattleArea(bool _IsBattleArea,TArray<class ASpawnPoint*> SpawnPoints,
			AActor* Camera,
			class UProceduralMeshComponent* LeftMesh,
			class UProceduralMeshComponent* RightMesh,
			class UProceduralMeshComponent* NearMesh
		);


	// �o�g���G���A����o��
	void ExitBattleArea();

	//�o�g���G���A�̃J����
	AActor* BattleAreaCamera;

	TArray<class UProceduralMeshComponent*> BattleAreaMeshs;

	TArray<class ASpawnPoint*> BattleAreaSpawnPoints;


	// �G�̏o������
	AActor* SpawnEnemy(ASpawnPoint* spawnPoint);

	// �o�g���G���A���ɓG���o��������
	void SpawnEnemyInBattleArea();

	// �Q�[�������̏�Ԃֈڂ����������������Ƃ�`����
	void SetNextGameState() { if (pGameState) { pGameState->SetReadyUpdateGame(true); } }

	// �v���C���[�ɃJ������ݒ肷��
	void SetCameraOnPlayer();

	// �G�l�~�[�̍폜
	void DestroyEnemy(AActor* _enemy, bool _bBattleAreaEnemy);

	// �f�t�H���g�|�[���N���X���擾
	TSubclassOf<APawn> GetDefaultPawnClass()const { return DefaultPawnClass; }

	// 
	void RestartGame();

private:

	// �o�g���G���A�̃t���O
	bool bIsOnBattleArea;

	// �o�g���G���A���̓G�̃J�E���^�[
	int32 BattleAreaEnemyCount = 0;

	// �v���C���[�̃J�����i�[�p(�ړ�����J����)
	AActor* PlayerCamera;

	// �v���C���[�̃J����(�J�����Œ莞)
	AActor* pCamera;

	// �Q�[���X�e�[�g�̃|�C���^
	class ANEOGameState* pGameState;

	// �v���C���[�R���g���[���[�̃|�C���^
	class ANEOPlayerController* PlayerController;
};
