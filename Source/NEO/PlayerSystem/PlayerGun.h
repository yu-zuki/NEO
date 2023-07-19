// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBase.h"
#include "PlayerGun.generated.h"

UCLASS()
class NEO_API APlayerGun : public APlayerBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerGun();

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
	virtual void SetupPlayerData() override;


	// 攻撃
	virtual void Attack(int AttackNum = 0) override;


	// 一つ目のコンボ
	virtual void Combo1() override;

	// 二つ目のコンボ
	virtual void Combo2()override;

};
