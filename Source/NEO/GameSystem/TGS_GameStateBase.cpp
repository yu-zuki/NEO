// �X�V���F2023/6/5		�X�V�ҁF��		�X�V���e�F�Q�[���̏�Ԃ��Ǘ�����N���X���쐬

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

//�Q�[���̏�Ԃ�����������
void ATGS_GameStateBase::InitGameState()
{
	PlayerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	//ECurrentPlayerType = EPlayerType::EPlayerType_1;
	
	//�o�g���G���A�̏�����
	InitBattleArea();

	//�Q�[���̏�Ԃ�����������
	InitCurrentState();

	//PlayerType������������
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
	//���\���ǂ��Ȃ��ꍇ�́A���L��For�����g��
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
	//���̃��x���̖��O���擾
	FString CurrentLevelName = GetWorld()->GetMapName();
	//���x�����̐擪�ɂ���"/Game/"���폜
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

	//�C���X�^���X����Q�[���̏�Ԃ�ǂݍ���
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
				Widget_PlayerStatus->AddToViewport();		//�v���C���[�̃X�e�[�^�X��\������
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
	// ��F�@/*if (player.hp < 0) { return true }*/

	return bIsGameOver;
}

void ATGS_GameStateBase::OnGameTitle()
{
	if (EchangeLevel == EChangeLevel::EChangeLevel_Playing)			//���x����ύX�ł��邩�ǂ���
	{
		SetCurrentState(EGameState::EGame_Playing);					//�Ӗ��Ȃ�����
		GetGameInstance()->SaveGameStateData(ECurrentState);		//�C���X�^���X�ɃQ�[���̏�Ԃ�ۑ�
		GetGameInstance()->SavePlayerType(ECurrentPlayerType);		//�C���X�^���X��PlayerType��ۑ�

		ChangeNextLevel(ENextLevel::ENextLevel_Playing);			//���x����ύX
	}

	ESubAction currentSubAction = UseSubAction();					//SubAction���擾
	//Enter�L�[����������A�Q�[�����J�n����
	if (currentSubAction == ESubAction::ESubAction_Enter) {
		EchangeLevel = EChangeLevel::EChangeLevel_Playing;
	}
	//PlayerType��I��
	else if (currentSubAction == ESubAction::ESubAction_Right) {
		//PlayerType��ύX����
		NextPlayerType();
	}
	else if (currentSubAction == ESubAction::ESubAction_Left) {
		//PlayerType��ύX����
		BackPlayerType();
	}

}


void ATGS_GameStateBase::OnGamePlaying(float DeltaTime)
{
	//Debug�p
	if (bIsOnBattleArea)	{
		EnterBattleArea();
		SetCurrentState(EGameState::EGame_InBattleArea);
		return;
	}


	if (IsGameOver() || EchangeLevel == EChangeLevel::EChangeLevel_Over) {
		SetCurrentState(EGameState::EGame_Over);
		GameEndTime = FDateTime::Now();

		GetGameInstance()->SaveGameStateData(ECurrentState);		//�C���X�^���X�ɃQ�[���̏�Ԃ�ۑ�
		ChangeNextLevel(ENextLevel::ENextLevel_Over);
	}
	else if (false) {
		SetCurrentState(EGameState::EGame_Pause);
	}
	else if (false) {
		SetCurrentState(EGameState::EGame_Menu);
	}

	//Enter�L�[�������ꂽ��A�Q�[����GameOver�ɂ���
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

		GetGameInstance()->SaveGameStateData(ECurrentState);		//�C���X�^���X�ɃQ�[���̏�Ԃ�ۑ�
		ChangeNextLevel(ENextLevel::ENextLevel_Title);
	}

	//Enter�L�[�������ꂽ��A�^�C�g���ɖ߂�
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

		GetGameInstance()->SaveGameStateData(ECurrentState);		//�C���X�^���X�ɃQ�[���̏�Ԃ�ۑ�
		ChangeNextLevel(ENextLevel::ENextLevel_Title);
	}

	//Menu��ʂ�ǉ�
}

void ATGS_GameStateBase::OnGamePause()
{
	if (true) {
		SetCurrentState(EGameState::EGame_Playing);
	}
}

void ATGS_GameStateBase::OnInBattleArea()
{
	//Debug�p BattleArea����o��
	if (UseSubAction() == ESubAction::ESubAction_Enter || BattleAreaEnemyCount <= 0) {
		//�o�g���G���A����o��
		ExitBattleArea();
		SetCurrentState(EGameState::EGame_Playing);
	}
	else if (IsGameOver()) {
		SetCurrentState(EGameState::EGame_Over);
		GameEndTime = FDateTime::Now();

		GetGameInstance()->SaveGameStateData(ECurrentState);		//�C���X�^���X�ɃQ�[���̏�Ԃ�ۑ�
		ChangeNextLevel(ENextLevel::ENextLevel_Over);
	}

}

void ATGS_GameStateBase::EnterBattleArea()
{
	//�o�g���G���A��L����
	for (auto Mesh : BattleAreaMeshs)	{
		if (Mesh)		{
			Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("MeshWall is not found"));
		}
	}

	//�v���C���[�̃J�������Œ�J�����ɕύX
	if (GetBattleAreaCamera()) {
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController) {
			PlayerController->SetViewTargetWithBlend(GetBattleAreaCamera(), 0.5f);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("PlayerController is not found"));
		}
	}

	//GameMode���g���āA�o�g���G���A�ɓG���X�|�[��������B�@���݌v��������
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
	//�o�g���G���A�𖳌���
	for (auto Mesh : BattleAreaMeshs) {
		if (Mesh) {
			Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("MeshWall is not found"));
		}
	}

	//�Œ�J�������v���C���[�̃J�����ɕύX
	if (PlayerCharacter) {
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController) {

			//Player�擾
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
	//�o�g���G���A�𖳌���
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
* �w�肳�ꂽNextLevel�p�����[�^�Ɋ�Â��ă��x����ύX���܂��B
* @param NextLevel ���ɕύX���郌�x�������肷��񋓒l�B
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
	//���̃��x���Ɉړ�����
	if (NextLevelName != NAME_None) {
		UGameplayStatics::OpenLevel(GetWorld(), NextLevelName);
	}
}

/**
* �w�肳�ꂽNextLevel�p�����[�^�Ɋ�Â��ă��x����ύX���܂��B
* @param NextLevel ���ɕύX���郌�x�������肷��񋓒l�B
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
 * @brief �T�u�A�N�V�������g�p���邽�߂̊֐�
 *
 * @details ���ݐݒ肳��Ă���T�u�A�N�V������Ԃ��A
 *          ���݂̃T�u�A�N�V���������Z�b�g����B
 *
 * @return ESubAction ���ݐݒ肳��Ă���T�u�A�N�V����
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

