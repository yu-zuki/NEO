// 更新日：2023/6/5		更新者：董		更新内容：ゲームモードの基本クラスを作成
// 更新日：2023/6/8		更新者：董		更新内容：ゲームモードのKeyInput機能、基本的な機能を作成
// 更新日：2023/7/20	更新者：董		更新内容：プレイヤーの選択機能を追加（GameModeのところに、プレイヤーのBPを設定、PlayStart　Objectが必須）
#include "TGS_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TGS_GameStateBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputCharacter.h"
#include "ProceduralMeshComponent.h"
#include "SpawnPoint.h"
#include "Engine/World.h"
#include "NEO/Enemy/EnamyBase.h"
#include "Ingame_WG.h"
#include "TGS_GameInstance.h"
#include "GameFramework/PlayerStart.h"
#include "../OdaBase.h"


ATGS_GameMode::ATGS_GameMode()
{
	SelctorPlayerType();
}

void ATGS_GameMode::BeginPlay()
{

	Super::BeginPlay();
	//ゲームステートを作成する
	GetGameState()->InitGameState();
	GetGameState()->GameStartTime = FDateTime::Now();

}

void ATGS_GameMode::StartPlay()
{
	SelctorPlayerType();

	Super::StartPlay();

}

void ATGS_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ゲームステートを更新する
	ATGS_GameStateBase* gameState = Cast<ATGS_GameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	if (gameState) {
		//gameState->SetSubAction(ESubAction::ESubAction_None);

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

void ATGS_GameMode::SpawnEnemyInBattleArea()
{
	//Check SpawnPoints
	if (GetGameState()->BattleAreaSpawnPoints.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("SpawnPoints is not found"));
		return;
	}

	//スポーン
	UWorld* World = GetWorld();
	if (!World) return;


	for (ASpawnPoint* spawnPoint : GetGameState()->BattleAreaSpawnPoints) {
		if (!spawnPoint) continue; //Check SpawnPoint

		//敵を生成する
		SpawnEnemy(spawnPoint);
	}
}

void ATGS_GameMode::SpawnEnemy(AActor* _enemy, FTransform _tranceform)
{
	//敵を生成する
	AActor* enemy = GetWorld()->SpawnActor<AActor>(EnemyBaseClass, _tranceform);

	//敵をゲームステートに登録する
	GetGameState()->AddEnemy(enemy);
}

AActor* ATGS_GameMode::SpawnEnemy(ASpawnPoint* spawnPoint)
{
	//NULL Check
	if (!spawnPoint) {
		UE_LOG(LogTemp, Error, TEXT("SpawnPoint is not found"));
		return nullptr;
	}

	//Transformを取得する
	FTransform spawnTransform = spawnPoint->GetTransform();

	//SpawnPointを使って、敵を生成する
	AOdaBase* Boss = Cast<AOdaBase>(GetWorld()->SpawnActor(spawnPoint->GetSpawnActorClass()));
	AEnamyBase* Enemy = Cast<AEnamyBase> ( GetWorld()->SpawnActor(spawnPoint->GetSpawnActorClass()) );
	if (Enemy)	{
		Enemy->SetActorTransform(spawnTransform);
		Enemy->IsAreaEnemy = true;		//Flag Set 

		//敵をゲームステートに登録する
		GetGameState()->AddEnemy(Enemy);

		GetGameState()->BattleAreaEnemyCount += 1;
		return Enemy;
	}
	else if(Boss){
		Boss->SetActorTransform(spawnTransform);
		Boss->IsAreaEnemy = true;		//Flag Set 

		//敵をゲームステートに登録する
		GetGameState()->AddEnemy(Boss);

		GetGameState()->BattleAreaEnemyCount += 1;
		return Boss;
	}

	return nullptr;

}

void ATGS_GameMode::DestroyEnemy(AActor* _enemy,bool BattleAreaEnemy)
{
	//敵をゲームステートから削除する
	GetGameState()->RemoveEnemy(_enemy);

	if(BattleAreaEnemy) {
		GetGameState()->BattleAreaEnemyCount -= 1;
	}
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

		//スポーンポイントをゲームステートに登録する
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

void ATGS_GameMode::SetUI_Enemy(FName _ActorName, int32 _NowHp, int32 _MaxHp)
{
	GetGameState()->Widget_GameMenu->SetWidgetValue(_ActorName, _NowHp, _MaxHp);
}

EPlayerType ATGS_GameMode::GetPlayerType()
{
	return GetGameState()->ECurrentPlayerType;
}

uint8 ATGS_GameMode::GetCurrentState()
{
	EGameState CurrentState = GetGameState()->GetCurrentState();
	return static_cast<uint8>(CurrentState);
}

void ATGS_GameMode::SelctorPlayerType()
{
	//NULL CHECK
	if (PlayerCharacterClassOne && PlayerCharacterClassTwo && PlayerCharacterClassThree) {

		UTGS_GameInstance* GameInstance = GetGameInstance();
		if (!GameInstance) {
			DefaultPawnClass = PlayerCharacterClassOne;
			UE_LOG(LogTemp, Error, TEXT("GameInstance is not found"));
			return;
		}

		AActor* SpawnPoint = GetPlayStartPoint();
		if (!SpawnPoint) {
			DefaultPawnClass = PlayerCharacterClassOne;
			UE_LOG(LogTemp, Error, TEXT("SpawnPoint is not found"));
			return;
		}

		FTransform SpawnPointT = SpawnPoint->GetTransform();

		//Defauly Pawn Classを設定する	
		switch (GetGameInstance()->LoadPlayerType())
		{
		case EPlayerType::EPlayerType_None:
			DefaultPawnClass = PlayerCharacterClassOne;
			return;
			break;
		case EPlayerType::EPlayerType_1:
			//DefaultPawnClass = PlayerCharacterClassOne;
			DefaultPawnClass = PlayerCharacterClassOne;
			break;
		case EPlayerType::EPlayerType_2:
			//DefaultPawnClass = PlayerCharacterClassTwo;
			DefaultPawnClass = PlayerCharacterClassTwo;
			break;
		case EPlayerType::EPlayerType_3:
			//DefaultPawnClass = PlayerCharacterClassThree;
			DefaultPawnClass = PlayerCharacterClassThree;
			break;
		}

		//プレイヤーを生成する
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController) {
			PlayerController->Possess(GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnPointT));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacterClass is not found"));
	}
}

//void ATGS_GameMode::BPFunction_Implementation()
//{
//	//GetInstance
//	UTGS_GameInstance* GameInstance = GetGameInstance();
//	if (GameInstance)	{
//		//LoadPlayerType
//		EPlayerType PlayerType = GameInstance->LoadPlayerType();
//		switch (PlayerType)
//		{
//		case EPlayerType::EPlayerType_None:
//			break;
//		case EPlayerType::EPlayerType_1:
//			break;
//		case EPlayerType::EPlayerType_2:
//			break;
//		case EPlayerType::EPlayerType_3:
//			break;
//		}
//	}
//}

AActor* ATGS_GameMode::GetPlayStartPoint()
{
	//スポーンポイントを取得する
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	if (Actors.Num() > 0) {
		return Actors[0];
	}
	return nullptr;
}

ATGS_GameStateBase* ATGS_GameMode::GetGameState()
{
	if (GameState == nullptr)
	{
		ATGS_GameStateBase* gameState = Cast<ATGS_GameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
		if (gameState) {
			GameState = gameState;
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Game State is not found"));
			return nullptr;
		}
	}

	return GameState;
}

UTGS_GameInstance* ATGS_GameMode::GetGameInstance()
{
	UTGS_GameInstance* GameInstance = Cast<UTGS_GameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance) {
		return GameInstance;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("GameInstance is not Found"));
	}
	return nullptr;
}