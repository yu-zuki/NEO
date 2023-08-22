// Fill out your copyright notice in the Description page of Project Settings.


#include "CurveTrigger.h"
#include "Components/BoxComponent.h"
#include "PlayerBase.h"

// Sets default values
ACurveTrigger::ACurveTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// タグ設定
	Tags.Add("CurveTrigger");

	CurveTriggerComp = CreateDefaultSubobject<UBoxComponent>("TriggerCollision");

	//OnActorBeginOverlap.AddDynamic(this, &ACurveTrigger::OnActorOverlapBegin);
	//OnActorEndOverlap.AddDynamic(this, &ACurveTrigger::OnActorOverlapEnd);
}

// Called when the game starts or when spawned
void ACurveTrigger::BeginPlay()
{
	Super::BeginPlay();

}

//// トリガーに当たったらカーブ開始
//void ACurveTrigger::OnActorOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
//{
//	// ヒットしたアクターが"Player"タグを持っていたら
//	if (OtherActor && (OtherActor != this) && OtherActor->ActorHasTag("Player"))
//	{
//		// カーブに突入したことをプレイヤーに知らせる
//		EnterCurve = true;
//	}
//}
//
//void ACurveTrigger::OnActorOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
//{
//	// ヒットしたアクターが"Player"タグを持っていたら
//	if (OtherActor && (OtherActor != this) && OtherActor->ActorHasTag("Player"))
//	{
//		// カーブが終了したことをプレイヤーに知らせる
//		EnterCurve = false;
//	}
//}
