// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBase.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class NEO_API APlayerCharacter : public APlayerBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//武器の当たり判定
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SwordCollision", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* WeaponCollision;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// プレイヤーのデータを初期化
	void SetupPlayerData() override;

};
