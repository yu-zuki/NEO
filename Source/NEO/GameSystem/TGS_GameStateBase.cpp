// 更新日：2023/6/5		更新者：董		更新内容：ゲームの状態を管理するクラスを作成

#include "TGS_GameStateBase.h"
#include "TGS_GameMode.h"
#include "TGS_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

#include "ProceduralMeshComponent.h"

//test
#include "JumpModuleActor.h"
#include "Ingame_WG.h"

ATGS_GameStateBase::ATGS_GameStateBase()
	:ECurrentState(EGameState::EGame_None), EchangeLevel(EChangeLevel::EChangeLevel_None), ECurrentPlayerType(EPlayerType::EPlayerType_None)
{

}

//ゲームの状態を初期化する
void ATGS_GameStateBase::InitGameState()
{
	PlayerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	//ECurrentPlayerType = EPlayerType::EPlayerType_1;
	
	//バトルエリアの初期化
	InitBattleArea();

	//ゲームの状態を初期化する
	InitCurrentState();

	//PlayerTypeを初期化する
	InitPlayerType();
}

void ATGS_GameStateBase::UpdateGameState(float DeltaTime)
{

	switch (ECurrentState)
	{
	case EGameState::EGame_None:
		break;
	case EGameState::EGame_Title:
		OnGameTitle();
		break;
	case EGameState::EGame_Playing:
		OnGamePlaying(DeltaTime);
		break;
	case EGameState::EGame_InBattleArea:
		OnInBattleArea();
		break;
	case EGameState::EGame_Over:
		OnGameOver();
		break;
	case EGameState::EGame_ReSpawnPlayer:
		OnReSpawnPlayer();
		break;
	case EGameState::EGame_Menu:
		OnGameMenu();
		break;
	case EGameState::EGame_Pause:
		OnGamePause();
		break;
	case EGameState::EGame_Unknown:
		break;
	default:
		break;
	}

	// EChangeLevel = EchangeLevel::EChangeLevel_None;
}

void ATGS_GameStateBase::AddEnemy(AActor* Enemy)
{
	Enemies.Add(Enemy);
}

void ATGS_GameStateBase::RemoveEnemy(AActor* Enemy)
{
	if (Enemy == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("RemoveEnemy is not Found"));
		return;
	}
	Enemy->Destroy();

	Enemies.Remove(Enemy);
}

TArray<class AActor*> ATGS_GameStateBase::GetEnemies() const
{
	return Enemies;
}

void ATGS_GameStateBase::ClearEnemies()
{
	//性能が良くない場合は、下記のFor文を使う
	for (auto Enemy : Enemies)
	{
		RemoveEnemy(Enemy);
	}

	//for (int i = 0; i < Enemies.Num(); i++)
	//{
	//	RemoveEnemy(Enemies[i]);
	//}
}

void ATGS_GameStateBase::InitCurrentState()
{
	//今のレベルの名前を取得
	FString CurrentLevelName = GetWorld()->GetMapName();
	//レベル名の先頭にある"/Game/"を削除
	CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	if ((FName)CurrentLevelName == GameTitleLevelName)
	{
		ECurrentState = EGameState::EGame_Title;
	}
	else if ((FName)CurrentLevelName == GamePlayLevelName)
	{
		ECurrentState = EGameState::EGame_Playing;
	}
	else if ((FName)CurrentLevelName == GameOverLevelName)
	{
		ECurrentState = EGameState::EGame_Over;
	}

	//インスタンスからゲームの状態を読み込む
	if (ECurrentState != GetGameInstance()->LoadGameStateData()) {
		GetGameInstance()->SaveGameStateData(ECurrentState);
		UE_LOG(LogTemp, Warning, TEXT("Not starting from GameTitle"));
	}		

	if (ECurrentState == EGameState::EGame_Playing)
	{
		if (Widget_GameMenuClass && Widget_PlayerStatusClass) {
			Widget_GameMenu = CreateWidget<UIngame_WG>(GetWorld(), Widget_GameMenuClass, "GameMenu");
			UUserWidget* Widget_PlayerStatus = CreateWidget<UUserWidget>(GetWorld(), Widget_PlayerStatusClass, "PlayerStatus");

			if (Widget_PlayerStatus) {
				Widget_PlayerStatus->AddToViewport();		//プレイヤーのステータスを表示する
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Widget_PlayerStatus is not found"));
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Widget_GameMenuClass or Widget_PlayerStatusClass is not found"));
		}
	}
}

void ATGS_GameStateBase::InitPlayerType()
{
	ECurrentPlayerType = GetGameInstance()->LoadPlayerType();
}

bool ATGS_GameStateBase::IsGameOver()
{
	// 例：　/*if (player.hp < 0) { return true }*/

	return bIsGameOver;
}

void ATGS_GameStateBase::OnGameTitle()
{
	if (EchangeLevel == EChangeLevel::EChangeLevel_Playing)			//レベルを変更できるかどうか
	{
		SetCurrentState(EGameState::EGame_Playing);					//意味ないかも
		GetGameInstance()->SaveGameStateData(ECurrentState);		//インスタンスにゲームの状態を保存
		GetGameInstance()->SavePlayerType(ECurrentPlayerType);		//インスタンスにPlayerTypeを保存

		ChangeNextLevel(ENextLevel::ENextLevel_Playing);			//レベルを変更
	}

	ESubAction currentSubAction = UseSubAction();					//SubActionを取得
	//Enterキーを押したら、ゲームを開始する
	if (currentSubAction == ESubAction::ESubAction_Enter) {
		EchangeLevel = EChangeLevel::EChangeLevel_Playing;
	}
	//PlayerTypeを選択
	else if (currentSubAction == ESubAction::ESubAction_Right) {
		//PlayerTypeを変更する
		NextPlayerType();
	}
	else if (currentSubAction == ESubAction::ESubAction_Left) {
		//PlayerTypeを変更する
		BackPlayerType();
	}

}


void ATGS_GameStateBase::OnGamePlaying(float DeltaTime)
{
	//Debug用
	if (bIsOnBattleArea)	{
		EnterBattleArea();
		SetCurrentState(EGameState::EGame_InBattleArea);
		return;
	}


	if (IsGameOver() || EchangeLevel == EChangeLevel::EChangeLevel_Over) {
		SetCurrentState(EGameState::EGame_Over);
		GameEndTime = FDateTime::Now();

		GetGameInstance()->SaveGameStateData(ECurrentState);		//インスタンスにゲームの状態を保存
		ChangeNextLevel(ENextLevel::ENextLevel_Over);
	}
	else if (false) {
		SetCurrentState(EGameState::EGame_Pause);
	}
	else if (false) {
		SetCurrentState(EGameState::EGame_Menu);
	}

	//Enterキーが押されたら、ゲームをGameOverにする
	if (UseSubAction() == ESubAction::ESubAction_Enter) {
		EchangeLevel = EChangeLevel::EChangeLevel_Over;
	}
}

void ATGS_GameStateBase::OnGameOver()
{
	if (false) {
		SetCurrentState(EGameState::EGame_ReSpawnPlayer);
	}
	else if (EchangeLevel == EChangeLevel::EChangeLevel_Title) {
		SetCurrentState(EGameState::EGame_Title);

		GetGameInstance()->SaveGameStateData(ECurrentState);		//インスタンスにゲームの状態を保存
		ChangeNextLevel(ENextLevel::ENextLevel_Title);
	}

	//Enterキーが押されたら、タイトルに戻る
	if (UseSubAction() == ESubAction::ESubAction_Enter) {
		EchangeLevel = EChangeLevel::EChangeLevel_Title;
	}
}

void ATGS_GameStateBase::OnReSpawnPlayer()
{
	ATGS_GameMode* GameMode = Cast<ATGS_GameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->RespawnPlayer();
	}
}

void ATGS_GameStateBase::OnGameMenu()
{
	if (EchangeLevel == EChangeLevel::EChangeLevel_Playing) {
		SetCurrentState(EGameState::EGame_Playing);
	}
	else if (true) {
		SetCurrentState(EGameState::EGame_Title);

		GetGameInstance()->SaveGameStateData(ECurrentState);		//インスタンスにゲームの状態を保存
		ChangeNextLevel(ENextLevel::ENextLevel_Title);
	}

	//Menu画面を追加
}

void ATGS_GameStateBase::OnGamePause()
{
	if (true) {
		SetCurrentState(EGameState::EGame_Playing);
	}
}

void ATGS_GameStateBase::OnInBattleArea()
{
	//Debug用 BattleAreaから出る
	if (UseSubAction() == ESubAction::ESubAction_Enter || BattleAreaEnemyCount <= 0) {
		//バトルエリアから出る
		ExitBattleArea();
		SetCurrentState(EGameState::EGame_Playing);
	}
	else if (IsGameOver()) {
		SetCurrentState(EGameState::EGame_Over);
		GameEndTime = FDateTime::Now();

		GetGameInstance()->SaveGameStateData(ECurrentState);		//インスタンスにゲームの状態を保存
		ChangeNextLevel(ENextLevel::ENextLevel_Over);
	}

}

void ATGS_GameStateBase::EnterBattleArea()
{
	//バトルエリアを有効化
	for (auto Mesh : BattleAreaMeshs)	{
		if (Mesh)		{
			Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("MeshWall is not found"));
		}
	}

	//プレイヤーのカメラを固定カメラに変更
	if (GetBattleAreaCamera()) {
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController) {
			PlayerController->SetViewTargetWithBlend(GetBattleAreaCamera(), 0.5f);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("PlayerController is not found"));
		}
	}

	//GameModeを使って、バトルエリアに敵をスポーンさせる。　＜設計が悪い＞
	ATGS_GameMode* GameMode = Cast<ATGS_GameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->SpawnEnemyInBattleArea();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("GameMode is not found"));
	}
}

void ATGS_GameStateBase::ExitBattleArea()
{
	bIsOnBattleArea = false;
	//バトルエリアを無効化
	for (auto Mesh : BattleAreaMeshs) {
		if (Mesh) {
			Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("MeshWall is not found"));
		}
	}

	//固定カメラをプレイヤーのカメラに変更
	if (PlayerCharacter) {
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController) {

			//Player取得
			ACharacter* tmp_Character = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0) );
			AActor* tmp_CameraActor = tmp_Character->GetOwner();
			if (tmp_CameraActor)			{
				PlayerController->SetViewTargetWithBlend(tmp_CameraActor, 0.5f);
			}
			else {
				//Log
				UE_LOG(LogTemp, Warning, TEXT("Player.GetOwner() : <CameraActor> is not found"));
			}

			
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("PlayerController is not found"));
		}
	}
}

void ATGS_GameStateBase::InitBattleArea()
{
	//バトルエリアを無効化
	ExitBattleArea();
}

AActor* ATGS_GameStateBase::GetBattleAreaCamera()
{
	if (BattleAreaCamera)	{
		return BattleAreaCamera;
	}
	else	{
		UE_LOG(LogTemp, Error, TEXT("BattleAreaCamera is not Found"));
		return nullptr;
	}
}

UTGS_GameInstance* ATGS_GameStateBase::GetGameInstance()
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

/**
* 指定されたNextLevelパラメータに基づいてレベルを変更します。
* @param NextLevel 次に変更するレベルを決定する列挙値。
*/
void ATGS_GameStateBase::ChangeNextLevel(ENextLevel NextLevel)
{
	switch (NextLevel)
	{
	case ENextLevel::ENextLevel_None:
		break;
	case ENextLevel::ENextLevel_Title:
		ChangeLevel(GameTitleLevelName);
		break;
	case ENextLevel::ENextLevel_Playing:
		ChangeLevel(GamePlayLevelName);
		break;
	case ENextLevel::ENextLevel_Over:
		ChangeLevel(GameOverLevelName);
		break;
	}
}

void ATGS_GameStateBase::ChangeLevel(FName NextLevelName)
{
	//次のレベルに移動する
	if (NextLevelName != NAME_None) {
		UGameplayStatics::OpenLevel(GetWorld(), NextLevelName);
	}
}

/**
* 指定されたNextLevelパラメータに基づいてレベルを変更します。
* @param NextLevel 次に変更するレベルを決定する列挙値。
*/
//EChangeLevel ATGS_GameStateBase::GetChangeLevel()
//{
//	EChangeLevel Temp = EchangeLevel;
//	EchangeLevel = EChangeLevel::EChangeLevel_None;
//
//	return Temp;
//}


void ATGS_GameStateBase::SetSubAction(ESubAction _subAction)
{
	CurrentSubAction = _subAction;
}

void ATGS_GameStateBase::NextPlayerType()
{
	if (ECurrentPlayerType == EPlayerType::EPlayerType_None)	{
		ECurrentPlayerType = EPlayerType::EPlayerType_1;
	}
	else if (ECurrentPlayerType == EPlayerType::EPlayerType_1) {
		ECurrentPlayerType = EPlayerType::EPlayerType_2;
	}
	else if (ECurrentPlayerType == EPlayerType::EPlayerType_2) {
		ECurrentPlayerType = EPlayerType::EPlayerType_3;
	}
	else if (ECurrentPlayerType == EPlayerType::EPlayerType_3) {
		ECurrentPlayerType = EPlayerType::EPlayerType_1;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ECurrentPlayerType is not found"));
	}
}

void ATGS_GameStateBase::BackPlayerType()
{
	if (ECurrentPlayerType == EPlayerType::EPlayerType_None) {
		ECurrentPlayerType = EPlayerType::EPlayerType_3;
	}
	else if (ECurrentPlayerType == EPlayerType::EPlayerType_1) {
		ECurrentPlayerType = EPlayerType::EPlayerType_3;
	}
	else if (ECurrentPlayerType == EPlayerType::EPlayerType_2) {
		ECurrentPlayerType = EPlayerType::EPlayerType_1;
	}
	else if (ECurrentPlayerType == EPlayerType::EPlayerType_3) {
		ECurrentPlayerType = EPlayerType::EPlayerType_2;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ECurrentPlayerType is not found"));
	}
}

/**
 * @brief サブアクションを使用するための関数
 *
 * @details 現在設定されているサブアクションを返し、
 *          現在のサブアクションをリセットする。
 *
 * @return ESubAction 現在設定されているサブアクション
 */
ESubAction ATGS_GameStateBase::UseSubAction()
{

	ESubAction tmp_SubAction = CurrentSubAction;
	CurrentSubAction = ESubAction::ESubAction_None;

	if (tmp_SubAction != ESubAction::ESubAction_None) {
		UE_LOG(LogTemp, Warning, TEXT("SubAction is %d"), (int)tmp_SubAction);
	}

	return tmp_SubAction;
}

