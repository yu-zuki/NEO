// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackAssistComponent.generated.h"


UCLASS( ClassGroup=(ActionAssist), meta=(BlueprintSpawnableComponent) )
class NEO_API UAttackAssistComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttackAssistComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 攻撃の角度修正
	void CorrectAttackAngle();

	// ヒットストップ処理
	void HitStop();

	// エフェクトスポーン
	void SpawnHitEffect();

	// オーナーをカメラに向ける
	void OwnerParallelToCamera(bool _lookRight);

private:

	// 前方にいるActorを返す
	AActor* GetFrontActor();

	// ヒットストップ終了
	void EndHitStop();


protected:

	//-------------各機能のON・OFF---------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Assist")
		bool bUseCorrectAttackAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Assist")
		bool bUseHitStop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Assist")
		bool bUseHitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Assist")
		bool bUseFaceCamera;

	//----------------------------------------------------------------------------

	// トレースの長さ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Assist")
		float LineLength;
		
	// HitStopで止まる時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Assist")
		float SpeedDuringHitStop;

	// HitStopで止まる時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Assist")
		float HitStopTime;

private:
	FTimerHandle TimerHandle_HitStop;
};
