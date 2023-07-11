// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.h"
#include "Bullet.h"
#include "EnamyBase.h"

#include "GunMan.generated.h"

UCLASS()
class NEO_API AGunMan : public AEnamyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGunMan();
	UPROPERTY(EditAnywhere, Category = "AI")
		float DesiredDistance = 500.0f; // プレイヤーからの望ましい距離（3m）

	UPROPERTY(EditAnywhere, Category = "AI")
		float MovementSpeed = 100.0f; // 移動速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* Death;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* Damage_Reaction;
	UPROPERTY()
		class ACharacter* PlayerCharacter; // プレイヤーキャラクターの参照
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float  Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float  MaxHealth;
	UFUNCTION(BlueprintCallable, Category = "Health")
		void ApplyDamage(float DamageAmount, float DeltaTime);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		float fJumpHeight;					//ジャンプ高さ

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		float fGravityAcceleration;			//重力加速度

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		float fJumpTime;					//ジャンプ時間

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		bool bIsJumping;					//ジャンプ中かどうか

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		FVector vJumpStartLocation;				//ジャンプ開始時の位置

	//ジャンプの開始
	UFUNCTION(BlueprintCallable, Category = "Jump")
		void StartJumpByGravity(float JumpHeight = 1500.f, float GravityAcceleration = 2000.f);

	//ジャンプ中
	UFUNCTION(BlueprintCallable, Category = "Jump")
		void JumpingByGravity(float DeltaTime);

	//ジャンプの終了
	UFUNCTION(BlueprintCallable, Category = "Jump")
		void EndJumpByGravity();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	int32 CallTracker = 5;
	UFUNCTION()
	void SpawnBullet();
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<class ABullet> BulletClass;

	FTimerHandle TimerHandle;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Rotation")
		bool bIsRotation;
	FVector GetPlayerDirection() const;
	
	float GetDistanceToPlayer() const;
	void FacePlayer();
};
