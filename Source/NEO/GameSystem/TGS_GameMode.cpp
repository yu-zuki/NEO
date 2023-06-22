// 更新日：2023/6/5		更新者：董		更新内容：ゲームモードの基本クラスを作成
// 更新日：2023/6/8		更新者：董		更新内容：ゲームモードのKeyInput機能、基本的な機能を作成

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
	//ゲームステートを作成する
	GetGameState()->InitGameState();
	GetGameState()->GameStartTime = FDateTime::Now();
}

void ATGS_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ゲームステートを更新する
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
	//次のレベルに移動する
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
	//敵を生成する
	AActor* enemy = GetWorld()->SpawnActor<AActor>(EnemyBaseClass, _tranceform);

	//敵をゲームステートに登録する
	GetGameState()->AddEnemy(enemy);
}

void ATGS_GameMode::DestroyEnemy(AActor* _enemy)
{
	//敵をゲームステートから削除する
	GetGameState()->RemoveEnemy(_enemy);

}

void ATGS_GameMode::ClearEnemy()
{
	//敵をゲームステートから削除する
	GetGameState()->ClearEnemies();
}

void ATGS_GameMode::SetState_GameOver()
{
	//ゲームステートをゲームオーバーにする
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
