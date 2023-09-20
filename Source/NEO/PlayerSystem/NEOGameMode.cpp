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

	// �Q�[���X�e�[�g���擾
	pGameState = Cast<ANEOGameState>(UGameplayStatics::GetGameState(GetWorld()));

	// �v���C���[�擾
	pPlayer = Cast<APlayerBase>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

void ANEOGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// �Q�[���̏�Ԃ��X�V
	if (pGameState) 
	{
		// �Q�[���̏�ԍX�V
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
	// �v���C���[�R���g���[���[���擾
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// �R���g���[���[�����鎞�J�������u�����h
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
		// �J�����̃R���|�[�l���g�擾
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

		//�X�|�[���|�C���g���Q�[���X�e�[�g�ɓo�^����
		BattleAreaSpawnPoints.Reset();
		for (auto spawnPoint : SpawnPoints) {
			BattleAreaSpawnPoints.Add(spawnPoint);
		}
	}


	//�o�g���G���A��L����
	for (auto Mesh : BattleAreaMeshs) {
		if (Mesh) {
			Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			//-------------------�R���W������L����----------------------

		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("MeshWall is not found"));
		}
	}

	//�v���C���[�̃J�������Œ�J�����ɕύX
	if (Camera) 
	{
		SetViewTargetWithBlend(Camera);
	}

	//GameMode���g���āA�o�g���G���A�ɓG���X�|�[��������B�@���݌v��������
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

	//Transform���擾����
	FTransform spawnTransform = spawnPoint->GetTransform();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* spawn_Actor = GetWorld()->SpawnActor<AActor>(spawnPoint->GetSpawnActorClass(), spawnPoint->GetActorLocation(), spawnPoint->GetActorRotation(), SpawnParams);


	AOdaBase* Boss = Cast<AOdaBase>(spawn_Actor);
	AEnamyBase* Enemy = Cast<AEnamyBase>(spawn_Actor);

	if (Enemy) {
		Enemy->SetActorTransform(spawnTransform);
		Enemy->IsAreaEnemy = true;		//Flag Set 

		// �G�l�~�[��ǉ�
		++BattleAreaEnemyCount;

		return Enemy;
	}
	else if (Boss) {
		Boss->SetActorTransform(spawnTransform);
		Boss->IsAreaEnemy = true;		//Flag Set 

		// �G�l�~�[��ǉ�
		++BattleAreaEnemyCount;

		return Boss;
	}

	return nullptr;

}

void ANEOGameMode::DestroyPlayer(AActor* _player)
{
	// �v���C���[�폜
	_player->Destroy();

	// �v���C���[��NULL��
	pPlayer = nullptr;

	// �Q�[�������̏�Ԃ�
	pGameState->SetNextGameState(EGameState_NEO::OnGameOver);
}

void ANEOGameMode::RespawnPlayer(AActor* _player)
{
	if (_player)
	{
		// �v���C���[�����S�����ʒu�擾
		FTransform RespownPos = _player->GetActorTransform();

		// �v���C���[���폜
		_player->Destroy();

		// �v���C���[��NULL��
		pPlayer = nullptr;

		// �V�����v���C���[�𐶐�
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
		// �G�l�~�[�폜
		_enemy->Destroy();

		// �G���A���̃G�l�~�[��������J�E���g�����炷
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

	//�X�|�[��
	UWorld* World = GetWorld();
	if (!World) return;


	for (ASpawnPoint* spawnPoint : BattleAreaSpawnPoints) {
		if (!spawnPoint) continue; //Check SpawnPoint

		//�G�𐶐�����
		SpawnEnemy(spawnPoint);
	}
}


void ANEOGameMode::ExitBattleArea()
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
	if (pPlayer)
	{
		if (PlayerCamera)
		{
			//�J�������v���C���[�̃J�����ɕύX
			SetViewTargetWithBlend(PlayerCamera, 1.f);
		}
	}
	else
	{
		//Log
		UE_LOG(LogTemp, Warning, TEXT("Player.GetOwner() : <CameraActor> is not found"));
	}
}