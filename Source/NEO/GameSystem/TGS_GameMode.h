// �X�V���F2023/6/5		�X�V�ҁF��		�X�V���e�F�Q�[�����[�h�̊�{�N���X���쐬
// �X�V���F2023/6/8		�X�V�ҁF��		�X�V���e�F�Q�[�����[�h��KeyInput�@�\�A��{�I�ȋ@�\���쐬

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameMode.h"
#include "InputCharacter.h"

#include "TGS_GameMode.generated.h"

enum class EChangeLevel : uint8;

/**
 *
 */
UCLASS()
class NEO_API ATGS_GameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ATGS_GameMode();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameClass")
		TSubclassOf<class AActor> PlayerBaseClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameClass")
		TSubclassOf<class AActor> EnemyBaseClass;


public:
	virtual void BeginPlay() override;

	virtual void StartPlay() override;

	void Tick(float DeltaTime) override;

	void NextLevel(FName _nextLevelName);

	UFUNCTION(BlueprintCallable, Category = "Level")
		void ChangeNextLevel(ENextLevel NextLevel);					//�ڕW��Level�ɑJ�ڂ���

//---------------------------------------�X�|�[�����f�X�g���C---------------------------------------
public:
	class APlayerBase* PlayerPtr;
	FTransform DeathTrans;

	void SpawnPlayer(AActor* _player, FTransform _tranceform);
	void DestroyPlayer(AActor* _player);
	void RespawnPlayer();

	//�o�g���G���A�ɂ���G���X�|�[������
	void SpawnEnemyInBattleArea();

	void SpawnEnemy(AActor* _enemy, FTransform _tranceform);
	AActor* SpawnEnemy(ASpawnPoint* spawnPoint);

	UFUNCTION(BlueprintCallable, Category = "GameSystem")
	void DestroyEnemy(AActor* _enemy, bool BattleAreaEnemy = false);

	void ClearEnemy();

//--------------------------------------------�L�[����---------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "GameSystem")
		void SetState_GameOver();

	void SetSubAction(ESubAction _eSubAction);

//-----------------------------------------�o�g���G���A-------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "Area")
		void SetIsOnBattleArea(bool bIsOnArea, TArray<class ASpawnPoint*> SpawnPoints, 
			AActor* Camera,
			class UProceduralMeshComponent* LeftMesh,
			class UProceduralMeshComponent* RightMesh, 
			class UProceduralMeshComponent* NearMesh
			);
	
	UFUNCTION(BlueprintCallable, Category = "Area")
		bool GetIsOnBattleArea();

	int32 GetBattleAreaEnemyNum();

//---------------------------------------�Q�[��UI---------------------------------------
public:
	void SetUI_Enemy(FName _ActorName, int32 _NowHp, int32 _MaxHp);

	UFUNCTION(BlueprintCallable, Category = "TitleUI")
		EPlayerType GetPlayerType();

	uint8 GetCurrentState();

	UFUNCTION(BlueprintCallable, Category = "GamerPlayerUI")
		float GetPlayerP();
///////////////////////////////////////////////////////////
///Player�I��
void SelctorPlayerType();

class UTGS_GameInstance* GetGameInstance();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
		TSubclassOf<class ACharacter> PlayerCharacterClassOne;			// �v���C���[1�̃L�����N�^�[
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
		TSubclassOf<class ACharacter> PlayerCharacterClassTwo;			// �v���C���[2�̃L�����N�^�[
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
		TSubclassOf<class ACharacter> PlayerCharacterClassThree;			// �v���C���[3�̃L�����N�^�[

	AActor* GetPlayStartPoint();

///////////////////////////////////////////////
///�J��������
public:
AActor* CameraActor;
FVector GetCameraLocation();
FRotator GetCameraRotation();
AActor* GetCameraActor();

//�g����
//�@ATGS_GameMode* GameMode = Cast<ATGS_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
//�@GameMode->SetViewTargetWithBlend(Actor, 0.5f);
void SetViewTargetWithBlend(class AActor* NewViewTarget, float BlendTime = 0, enum EViewTargetBlendFunction BlendFunc = VTBlend_Linear, float BlendExp = 0, bool bLockOutgoing = false);

protected:
	UFUNCTION(BlueprintCallable, Category = "GameSystem")
	class ATGS_GameStateBase* GetGameState();

	ATGS_GameStateBase* GameState = nullptr;		//�L���V���[


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Area)
		bool NowBattleArea;

		
};
