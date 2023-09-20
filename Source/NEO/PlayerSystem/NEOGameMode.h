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

	// �v���C���[�̃J�����ݒ�
	void SetPlayerCamera(AActor* _playerCamera) { PlayerCamera = _playerCamera; }
	
	// ���݂̃J�����擾
	AActor* GetPlayerCamera()const { return pCamera; }

	FRotator GetCameraRotation()const;

	UFUNCTION(BlueprintCallable, Category = "Area")
		void SetIsOnBattleArea(bool _IsBattleArea,TArray<class ASpawnPoint*> SpawnPoints,
			AActor* Camera,
			class UProceduralMeshComponent* LeftMesh,
			class UProceduralMeshComponent* RightMesh,
			class UProceduralMeshComponent* NearMesh
		);


	void ExitBattleArea();

	//�o�g���G���A�̃J����
	AActor* BattleAreaCamera;

	TArray<class UProceduralMeshComponent*> BattleAreaMeshs;

	TArray<class ASpawnPoint*> BattleAreaSpawnPoints;

	// �o�g���G���A�̃t���O
	bool bIsOnBattleArea;

	// �o�g���G���A�ɂ���G���X�|�[������
	AActor* SpawnEnemy(ASpawnPoint* spawnPoint);

	// 
	void SpawnEnemyInBattleArea();

	// �v���C���[�̍폜
	void DestroyPlayer(AActor* _player);

	// �v���C���[�̍폜
	void RespawnPlayer(AActor* _player);


	// �G�l�~�[�̍폜
	void DestroyEnemy(AActor* _enemy, bool _bBattleAreaEnemy);


private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int32 BattleAreaEnemyCount = 0;

	// �v���C���[�̃J����
	AActor* PlayerCamera;

	// 
	AActor* pCamera;

	// �Q�[���X�e�[�g�̃|�C���^
	class ANEOGameState* pGameState;

	// �v���C���[�R���g���[���[�̃|�C���^
	class APlayerController* PlayerController;

	// �v���C���[�̃|�C���^
	class APlayerBase* pPlayer;
};
