// Fill out your copyright notice in the Description page of Project Settings.


#include "NEOGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "NEOGameState.h"
#include "Camera/CameraComponent.h"
#include "ProceduralMeshComponent.h"
#include "NEO/GameSystem/SplineCamera.h"
#include "NEO/GameSystem/SpawnPoint.h"
#include "NEO/OdaBase.h"
#include "NEO/Enemy/EnamyBase.h"
#include "PlayerBase.h"


ANEOGameMode::ANEOGameMode()
{

}


void ANEOGameMode::BeginPlay()
{
	Super::BeginPlay();

	// ゲームステートを取得
	pGameState = Cast<ANEOGameState>(UGameplayStatics::GetGameState(GetWorld()));

	// プレイヤー取得
	pPlayer = Cast<APlayerBase>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

void ANEOGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// ゲームの状態を更新
	if (pGameState) 
	{
		// ゲームの状態更新
		pGameState->UpdateGameState(DeltaTime);
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Game State is not found"));
		pGameState = Cast<ANEOGameState>(UGameplayStatics::GetGameState(GetWorld()));
	}
}

void ANEOGameMode::SetViewTargetWithBlend(AActor* _newViewTarget, float _blendTime, EViewTargetBlendFunction _blendFunc, float _blendExp, bool _bLockOutgoing)
{
	// プレイヤーコントローラーを取得
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// コントローラーがある時カメラをブレンド
	if (PlayerController)
	{
		PlayerController->SetViewTargetWithBlend(_newViewTarget, _blendTime);
	}

	pCamera = _newViewTarget;
}

FRotator ANEOGameMode::GetCameraRotation()const
{
	if (pCamera)
	{
		// カメラのコンポーネント取得
		UCameraComponent* CameraComponent = pCamera->FindComponentByClass<UCameraComponent>();
		if (CameraComponent) 
		{
			return CameraComponent->GetComponentRotation();
		}
	}

	return FRotator::ZeroRotator;
}



void ANEOGameMode::SetIsOnBattleArea(bool _IsBattleArea,TArray<class ASpawnPoint*> SpawnPoints,
	AActor* Camera = nullptr,
	class UProceduralMeshComponent* LeftMesh = nullptr,
	class UProceduralMeshComponent* RightMesh = nullptr,
	class UProceduralMeshComponent* NearMesh = nullptr)
{
	// NULL Check
	if (Camera && LeftMesh && RightMesh && NearMesh) {
		BattleAreaMeshs.Reset();

		BattleAreaMeshs.Add(LeftMesh);
		BattleAreaMeshs.Add(RightMesh);
		BattleAreaMeshs.Add(NearMesh);

		BattleAreaCamera = Camera;

		//スポーンポイントをゲームステートに登録する
		BattleAreaSpawnPoints.Reset();
		for (auto spawnPoint : SpawnPoints) {
			BattleAreaSpawnPoints.Add(spawnPoint);
		}
	}


	//バトルエリアを有効化
	for (auto Mesh : BattleAreaMeshs) {
		if (Mesh) {
			Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			//-------------------コリジョンを有効化----------------------

		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("MeshWall is not found"));
		}
	}

	//プレイヤーのカメラを固定カメラに変更
	if (Camera) 
	{
		SetViewTargetWithBlend(Camera);
	}

	//GameModeを使って、バトルエリアに敵をスポーンさせる。　＜設計が悪い＞
	SpawnEnemyInBattleArea();

	bIsOnBattleArea = _IsBattleArea;
}

AActor* ANEOGameMode::SpawnEnemy(ASpawnPoint* spawnPoint)
{
	//NULL Check
	if (!spawnPoint) {
		UE_LOG(LogTemp, Error, TEXT("SpawnPoint is not found"));
		return nullptr;
	}

	//Transformを取得する
	FTransform spawnTransform = spawnPoint->GetTransform();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* spawn_Actor = GetWorld()->SpawnActor<AActor>(spawnPoint->GetSpawnActorClass(), spawnPoint->GetActorLocation(), spawnPoint->GetActorRotation(), SpawnParams);


	AOdaBase* Boss = Cast<AOdaBase>(spawn_Actor);
	AEnamyBase* Enemy = Cast<AEnamyBase>(spawn_Actor);

	if (Enemy) {
		Enemy->SetActorTransform(spawnTransform);
		Enemy->IsAreaEnemy = true;		//Flag Set 

		// エネミーを追加
		++BattleAreaEnemyCount;

		return Enemy;
	}
	else if (Boss) {
		Boss->SetActorTransform(spawnTransform);
		Boss->IsAreaEnemy = true;		//Flag Set 

		// エネミーを追加
		++BattleAreaEnemyCount;

		return Boss;
	}

	return nullptr;

}

void ANEOGameMode::DestroyPlayer(AActor* _player)
{
	// プレイヤー削除
	_player->Destroy();

	// プレイヤーをNULLに
	pPlayer = nullptr;

	// ゲームを次の状態へ
	pGameState->SetNextGameState(EGameState_NEO::OnGameOver);
}

void ANEOGameMode::RespawnPlayer(AActor* _player)
{
	if (_player)
	{
		// プレイヤーが死亡した位置取得
		FTransform RespownPos = _player->GetActorTransform();

		// プレイヤーを削除
		_player->Destroy();

		// プレイヤーをNULLに
		pPlayer = nullptr;

		// 新しいプレイヤーを生成
		if (PlayerController) {
			pPlayer = Cast<APlayerBase>(GetWorld()->SpawnActor<APawn>(DefaultPawnClass, RespownPos));

			AActor* NowCamera = (bIsOnBattleArea)?(pCamera):(PlayerCamera);

			SetViewTargetWithBlend(NowCamera);
		}
	}
}


void ANEOGameMode::DestroyEnemy(AActor* _enemy, bool _bBattleAreaEnemy)
{
	if (_enemy)
	{
		// エネミー削除
		_enemy->Destroy();

		// エリア内のエネミーだったらカウントを減らす
		if (_bBattleAreaEnemy)
		{
			--BattleAreaEnemyCount;

			if (BattleAreaEnemyCount == 0 && bIsOnBattleArea)
			{
				ExitBattleArea();
			}
		}
	}
}


void ANEOGameMode::SpawnEnemyInBattleArea()
{
	//Check SpawnPoints
	if (BattleAreaSpawnPoints.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("SpawnPoints is not found"));
		return;
	}

	//スポーン
	UWorld* World = GetWorld();
	if (!World) return;


	for (ASpawnPoint* spawnPoint : BattleAreaSpawnPoints) {
		if (!spawnPoint) continue; //Check SpawnPoint

		//敵を生成する
		SpawnEnemy(spawnPoint);
	}
}


void ANEOGameMode::ExitBattleArea()
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
	if (pPlayer)
	{
		if (PlayerCamera)
		{
			//カメラをプレイヤーのカメラに変更
			SetViewTargetWithBlend(PlayerCamera, 1.f);
		}
	}
	else
	{
		//Log
		UE_LOG(LogTemp, Warning, TEXT("Player.GetOwner() : <CameraActor> is not found"));
	}
}