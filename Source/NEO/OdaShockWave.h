// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "OdaShockWave.generated.h"

UCLASS()
class NEO_API AOdaShockWave : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOdaShockWave();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//�v���C���[���E���ɂ��邩
	bool IsRightPlayer;

	//�G�t�F�N�g�𓮂�������
	void MoveWave();

	//NiagaraSystem������
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* Wave;

	UPROPERTY(EditAnywhere)
		class UNiagaraComponent* NiagaraComp;

	//�G�t�F�N�g�̑傫�������߂�
	UPROPERTY(EditAnywhere)
	FVector Scale;


};
