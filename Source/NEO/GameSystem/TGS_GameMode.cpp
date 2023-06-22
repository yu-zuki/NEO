// �X�V���F2023/6/5		�X�V�ҁF��		�X�V���e�F�Q�[�����[�h�̊�{�N���X���쐬
// �X�V���F2023/6/8		�X�V�ҁF��		�X�V���e�F�Q�[�����[�h��KeyInput�@�\�A��{�I�ȋ@�\���쐬

#include "TGS_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TGS_GameStateBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputCharacter.h"


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

void ATGS_GameMode::SpawnEnemy(AActor* _enemy, FTransform _tranceform)
{
	//�G�𐶐�����
	AActor* enemy = GetWorld()->SpawnActor<AActor>(EnemyBaseClass, _tranceform);

	//�G���Q�[���X�e�[�g�ɓo�^����
	GetGameState()->AddEnemy(enemy);
}

void ATGS_GameMode::DestroyEnemy(AActor* _enemy)
{
	//�G���Q�[���X�e�[�g����폜����
	GetGameState()->RemoveEnemy(_enemy);

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

void ATGS_GameMode::SetIsOnBattleArea(bool bIsOnArea,AActor* Camera)
{
	GetGameState()->bIsOnBattleArea = bIsOnArea; 
	GetGameState()->BattleAreaCamera = Camera;
}

bool ATGS_GameMode::GetIsOnBattleArea()
{
	return GetGameState()->bIsOnBattleArea;
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
