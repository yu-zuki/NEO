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
	// Sets default values for this actor's properties
	ACurveTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	UFUNCTION()
		virtual void OnActorOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		virtual void OnActorOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		bool GetCurve() { return EnterCurve; }



private:
	// 判定用のボックス
	class UBoxComponent* CurveTriggerComp;

	// カーブに入ったことを知らせる
	bool EnterCurve;
};
