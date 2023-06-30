// �X�V���F2023/6/5		�X�V�ҁF��		�X�V���e�F�Q�[�����[�h�̊�{�N���X���쐬
// �X�V���F2023/6/8		�X�V�ҁF��		�X�V���e�F�Q�[�����[�h��KeyInput�@�\�A��{�I�ȋ@�\���쐬

#include "TGS_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TGS_GameStateBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputCharacter.h"
#include "ProceduralMeshComponent.h"
#include "SpawnPoint.h"
#include "Engine/World.h"

ATGS_GameMode::ATGS_GameMode()
{
}

void ATGS_GameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ATGS_GameMode::StartPlay()
{
	Super::StartPlay();
	//�Q�[���X�e�[�g���쐬����
	GetGameState()->InitGameState();
	GetGameState()->GameStartTime = FDateTime::Now();
}

void ATGS_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�Q�[���X�e�[�g���X�V����
	ATGS_GameStateBase* gameState = Cast<ATGS_GameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if (gameState) {
		gameState->UpdateGameState(DeltaTime);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Game State is not found"));
	}

}

void ATGS_GameMode::NextLevel(FName _nextLevelName)
{
	//���̃��x���Ɉړ�����
	if (_nextLevelName != NAME_None) {
		UGameplayStatics::OpenLevel(GetWorld(), _nextLevelName);
	}
}

void ATGS_GameMode::ChangeNextLevel(ENextLevel NextLevel)
{
	GetGameState()->ChangeNextLevel(NextLevel);
}

void ATGS_GameMode::SpawnPlayer(AActor* _player, FTransform _tranceform)
{
	GetWorld()->SpawnActor<AActor>(PlayerBaseClass, _tranceform);
}

void ATGS_GameMode::DestroyPlayer(AActor* _player)
{

}

void ATGS_GameMode::RespawnPlayer()
{
}

void ATGS_GameMode::SpawnEnemyInBattleArea()
{
	//Check SpawnPoints
	if (GetGameState()->BattleAreaSpawnPoints.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("SpawnPoints is not found"));
		return;
	}

	//�X�|�[��
	UWorld* World = GetWorld();
	if (!World) return;


	for (ASpawnPoint* spawnPoint : GetGameState()->BattleAreaSpawnPoints) {
		if (!spawnPoint) continue; //Check SpawnPoint

		//�G�𐶐�����
		SpawnEnemy(spawnPoint);
	}
}

void ATGS_GameMode::SpawnEnemy(AActor* _enemy, FTransform _tranceform)
{
	//�G�𐶐�����
	AActor* enemy = GetWorld()->SpawnActor<AActor>(EnemyBaseClass, _tranceform);

	//�G���Q�[���X�e�[�g�ɓo�^����
	GetGameState()->AddEnemy(enemy);
}

AActor* ATGS_GameMode::SpawnEnemy(ASpawnPoint* spawnPoint)
{
	//NULL Check
	if (!spawnPoint) {
		UE_LOG(LogTemp, Error, TEXT("SpawnPoint is not found"));
		return nullptr;
	}

	//Transform���擾����
	FTransform spawnTransform = spawnPoint->GetTransform();

	//SpawnPoint���g���āA�G�𐶐�����
	AActor* Enemy = GetWorld()->SpawnActor(spawnPoint->GetSpawnActorClass());
	Enemy->SetActorTransform(spawnTransform);

	//�G���Q�[���X�e�[�g�ɓo�^����
	GetGameState()->AddEnemy(Enemy);

	GetGameState()->BattleAreaEnemyCount += 1;

	return Enemy;
}

void ATGS_GameMode::DestroyEnemy(AActor* _enemy,bool BattleAreaEnemy)
{
	//�G���Q�[���X�e�[�g����폜����
	GetGameState()->RemoveEnemy(_enemy);

	if(BattleAreaEnemy) {
		GetGameState()->BattleAreaEnemyCount -= 1;
	}
}

void ATGS_GameMode::ClearEnemy()
{
	//�G���Q�[���X�e�[�g����폜����
	GetGameState()->ClearEnemies();
}

void ATGS_GameMode::SetState_GameOver()
{
	//�Q�[���X�e�[�g���Q�[���I�[�o�[�ɂ���
	GetGameState()->bIsGameOver = true;
}

void ATGS_GameMode::SetSubAction(ESubAction _eSubAction)
{
	GetGameState()->SetSubAction(_eSubAction);
}

void ATGS_GameMode::SetIsOnBattleArea(bool bIsOnArea, TArray<class ASpawnPoint*> SpawnPoints,
	AActor* Camera = nullptr,
	class UProceduralMeshComponent* LeftMesh = nullptr,
	class UProceduralMeshComponent* RightMesh = nullptr,
	class UProceduralMeshComponent* NearMesh = nullptr)
{
	//NULL Check
	if (Camera && LeftMesh && RightMesh && NearMesh) {
		GetGameState()->BattleAreaMeshs.Reset();

		GetGameState()->BattleAreaMeshs.Add(LeftMesh);
		GetGameState()->BattleAreaMeshs.Add(RightMesh);
		GetGameState()->BattleAreaMeshs.Add(NearMesh);

		GetGameState()->BattleAreaCamera = Camera;

		//�X�|�[���|�C���g���Q�[���X�e�[�g�ɓo�^����
		GetGameState()->BattleAreaSpawnPoints.Reset();
		for(auto spawnPoint : SpawnPoints) {
			GetGameState()->BattleAreaSpawnPoints.Add(spawnPoint);
		}
	}

	GetGameState()->bIsOnBattleArea = bIsOnArea; 
}

bool ATGS_GameMode::GetIsOnBattleArea()
{
	return GetGameState()->bIsOnBattleArea;
}

int32 ATGS_GameMode::GetBattleAreaEnemyNum()
{
	 return GetGameState()->BattleAreaEnemyCount;
}

ATGS_GameStateBase* ATGS_GameMode::GetGameState()
{
	ATGS_GameStateBase* gameState = Cast<ATGS_GameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if(gameState) {
		return gameState;
	}
	else	{
		UE_LOG(LogTemp, Error, TEXT("Game State is not found"));
		return nullptr;
	}

}
