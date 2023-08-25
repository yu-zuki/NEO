// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "CurveTrigger.generated.h"

/**
 * 
 */
UCLASS()
class NEO_API ACurveTrigger : public ATriggerBox
{
	GENERATED_BODY()
	
public:

	// コンストラクタ
	ACurveTrigger();

	// ゲーム開始時に呼び出される処理
	virtual void BeginPlay() override;

	//// このActorに衝突した瞬間に行う処理
	//virtual void OnActorOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	//// このActorに衝突し終わる瞬間に行う処理
	//virtual void OnActorOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);


private:

	// 判定用のボックスコンポーネント
	class UBoxComponent* CurveTriggerComp;
};
