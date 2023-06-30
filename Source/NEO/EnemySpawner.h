// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnamyBase.h" // �X�|�[������Enemy�L�����N�^�[�̃w�b�_�[�t�@�C�����C���N���[�h���Ă�������
#include "EnemySpawner.generated.h"

UCLASS()
class NEO_API AEnemySpawner : public AActor
{
    GENERATED_BODY()

public:
    AEnemySpawner();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Spawning")
        TSubclassOf<AEnamyBase> EnemyClass;

    UPROPERTY(EditAnywhere, Category = "Spawning")
        int32 NumEnemiesToSpawn;

   

    void SpawnEnemies();
};
