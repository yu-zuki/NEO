// �X�V���F2023/6/5		�X�V�ҁF��		�X�V���e�F�Q�[�����[�h�̊�{�N���X���쐬
// �X�V���F2023/6/8		�X�V�ҁF��		�X�V���e�F�Q�[�����[�h��KeyInput�@�\�A��{�I�ȋ@�\���쐬
// �X�V���F2023/7/20	�X�V�ҁF��		�X�V���e�F�v���C���[�̑I���@�\��ǉ��iGameMode�̂Ƃ���ɁA�v���C���[��BP��ݒ�APlayStart�@Object���K�{�j
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
#include "Camera/CameraComponent.h"


ATGS_GameMode::ATGS_GameMode()
{
	SelctorPlayerType();
}

void ATGS_GameMode::BeginPlay()
{

	Super::BeginPlay();
	//�Q�[���X�e�[�g���쐬����
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

	//�Q�[���X�e�[�g���X�V����
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
	DeathTrans = _player->GetActorTransform();
	_player->Destroy();
}

void ATGS_GameMode::RespawnPlayer()
{
	//�v���C���[�𐶐�����
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) {
		PlayerPtr = Cast<APlayerBase>(GetWorld()->SpawnActor<APawn>(DefaultPawnClass, DeathTrans));
		PlayerController->Possess(PlayerPtr);

		SetViewTargetWithBlend(GetCameraActor());

	}
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
	AActor* spawn_Actor = GetWorld()->SpawnActor(spawnPoint->GetSpawnActorClass());

	AOdaBase* Boss = Cast<AOdaBase>(spawn_Actor);
	AEnamyBase* Enemy = Cast<AEnamyBase> (spawn_Actor);

	if (Enemy)	{
		Enemy->SetActorTransform(spawnTransform);
		Enemy->IsAreaEnemy = true;		//Flag Set 

		//�G���Q�[���X�e�[�g�ɓo�^����
		GetGameState()->AddEnemy(Enemy);

		GetGameState()->BattleAreaEnemyCount += 1;
		return Enemy;
	}
	else if(Boss){
		Boss->SetActorTransform(spawnTransform);
		Boss->IsAreaEnemy = true;		//Flag Set 

		//�G���Q�[���X�e�[�g�ɓo�^����
		GetGameState()->AddEnemy(Boss);

		GetGameState()->BattleAreaEnemyCount += 1;
		return Boss;
	}

	return nullptr;

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

void ATGS_GameMode::SetState_GameOver(float _time)
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

void ATGS_GameMode::SetUI_Enemy(FName _ActorName, int32 _NowHp, int32 _MaxHp)
{
	//GetGameState()->Widget_GameMenu->SetWidgetValue(_ActorName, _NowHp, _MaxHp);
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

float ATGS_GameMode::GetPlayerP()
{	
	return PlayerPtr ? PlayerPtr->GetPlayerHPPercent() : 0 ;
}

void ATGS_GameMode::SelctorPlayerType()
{
	//NULL CHECK
	if (PlayerCharacterClassOne && PlayerCharacterClassTwo && PlayerCharacterClassThree) {

		UTGS_GameInstance* GameInstance = GetGameInstance();
		if (!GameInstance) {
			DefaultPawnClass = PlayerCharacterClassOne;
			UE_LOG(LogTemp, Warning, TEXT("GameInstance is not found"));
			return;
		}

		AActor* SpawnPoint = GetPlayStartPoint();
		if (!SpawnPoint) {
			DefaultPawnClass = PlayerCharacterClassOne;
			UE_LOG(LogTemp, Warning, TEXT("SpawnPoint is not found"));
			return;
		}

		FTransform SpawnPointT = SpawnPoint->GetTransform();

		//Defauly Pawn Class��ݒ肷��	
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

		//�v���C���[�𐶐�����
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController) {
			PlayerPtr = Cast<APlayerBase>(GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnPointT));
			PlayerController->Possess(PlayerPtr);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacterClass is not found"));
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
	//�X�|�[���|�C���g���擾����
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	if (Actors.Num() > 0) {
		return Actors[0];
	}
	return nullptr;
}

FVector ATGS_GameMode::GetCameraLocation()
{
	if (CameraActor) {
		//CameraComponent Get
		UCameraComponent* CameraComponent = CameraActor->FindComponentByClass<UCameraComponent>();
		if (CameraComponent) {
			return CameraComponent->GetComponentLocation();
		}
	}

	return FVector();
}

FRotator ATGS_GameMode::GetCameraRotation()
{
	if (CameraActor) {
		//CameraComponent Get
		UCameraComponent* CameraComponent = CameraActor->FindComponentByClass<UCameraComponent>();
		if (CameraComponent) {
			return CameraComponent->GetComponentRotation();
		}
	}

	return FRotator();
}


AActor* ATGS_GameMode::GetCameraActor()
{
	return CameraActor;
}

void ATGS_GameMode::SetViewTargetWithBlend(AActor* NewViewTarget, float BlendTime, EViewTargetBlendFunction BlendFunc, float BlendExp, bool bLockOutgoing)
{
	//NULL Check
	if (!NewViewTarget) {
		UE_LOG(LogTemp, Error, TEXT("NewViewTarget is not found"));
		return;
	}

	//PlayerController�擾
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (!PlayerController) {
		UE_LOG(LogTemp, Error, TEXT("PlayerController is not found"));
		return;
	}

	//Camera�̍��W���L�^
	CameraActor = NewViewTarget;

	PlayerController->SetViewTargetWithBlend(NewViewTarget, BlendTime);


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
	UWorld* World = GetWorld();
	if(!World) return nullptr;

	UTGS_GameInstance* GameInstance = Cast<UTGS_GameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance) {
		return GameInstance;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("GameInstance is not Found"));
	}
	return nullptr;
}