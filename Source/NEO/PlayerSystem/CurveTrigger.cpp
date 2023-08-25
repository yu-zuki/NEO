// Fill out your copyright notice in the Description page of Project Settings.


#include "CurveTrigger.h"
#include "Components/BoxComponent.h"
#include "PlayerBase.h"

// Sets default values
ACurveTrigger::ACurveTrigger()
{
	// Tick()を毎フレーム呼ばないようにする
	PrimaryActorTick.bCanEverTick = false;

	// タグ設定
	Tags.Add("CurveTrigger");

	CurveTriggerComp = CreateDefaultSubobject<UBoxComponent>("TriggerCollision");

	/*OnActorBeginOverlap.AddDynamic(this, &ACurveTrigger::OnActorOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ACurveTrigger::OnActorOverlapEnd);*/
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
//		// プレイヤーの情報取得
//		APlayerBase* pPlayer = Cast<APlayerBase>(OtherActor);
//
//		// カーブに入ったことを知らせる
//		if (pPlayer)
//		{
//			//pPlayer->
//		}
//	}
//}
//
//void ACurveTrigger::OnActorOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
//{
//	// ヒットしたアクターが"Player"タグを持っていたら
//	if (OtherActor && (OtherActor != this) && OtherActor->ActorHasTag("Player"))
//	{
//		// カーブが終了したことをプレイヤーに知らせる
//		//EnterCurve = false;
//	}
//}
