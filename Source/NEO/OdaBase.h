// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "OdaBase.generated.h"

UCLASS()
class NEO_API AOdaBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOdaBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//ボスキャラクター
	UPROPERTY()
	AOdaBase* OdaNobunaga;

	//キャラクタームーブメント
	UPROPERTY()
		UCharacterMovementComponent* NobunagaMovement;
	
	//前方向をとる
	UPROPERTY()
	FVector ForwardDirection;

	//移動速度
	UPROPERTY(EditAnywhere)
		float OdaSpeed;
};
