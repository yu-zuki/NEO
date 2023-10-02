// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NEO/GameSystem/SpawnPoint.h"
#include "OdaBase.h"
#include "Enemy/EnamyBase.h"
#include "Templates/SubclassOf.h"								//TSubclassOf���g�����߂ɕK�v�ȃw�b�_�[
#include "EnemyAIController.generated.h"

/**
 *
 */
UCLASS()
class NEO_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
		AEnemyAIController();
	//��邱��
	//�G�̏o���폜
	//�{�X�A�G�̃}�b�v�ړ�������

	//�֐�
	//�G�̏o��------------------------------------------------
		AActor* EnemySpawn(ASpawnPoint* spawnPoint);



	//�ϐ�
	//�G�̏o��------------------------------------------------
	UPROPERTY(EditAnywhere)
	TSubclassOf< class AActor >EnemyInfo = nullptr;


};
