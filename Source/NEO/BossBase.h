// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "BossBase.generated.h"

UCLASS()
class NEO_API ABossBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Sceneコンポーネント
	UPROPERTY(EditAnywhere)
	USceneComponent* DefaultSceneRoot;

	FString Skeletal_Path = TEXT("SkeletalMesh'/Game/Enemy/BOSS/Boss_Mesh.BossMesh'");



	

};
