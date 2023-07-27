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

	OnActorBeginOverlap.AddDynamic(this, &ACurveTrigger::OnActorOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ACurveTrigger::OnActorOverlapEnd);
}

// Called when the game starts or when spawned
void ACurveTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACurveTrigger::OnActorOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	// ヒットしたアクターが"Player"タグを持っていたら
	if (OtherActor && (OtherActor != this) && OtherActor->ActorHasTag("Player"))
	{
		// プレイヤー情報格納
		APlayerBase* Player = Cast<APlayerBase>(OtherActor);

		// カーブに突入したことをプレイヤーに知らせる
		Player->OnCurveMode(true);
	}
}

void ACurveTrigger::OnActorOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	// ヒットしたアクターが"Player"タグを持っていたら
	if (OtherActor && (OtherActor != this) && OtherActor->ActorHasTag("Player"))
	{
		// プレイヤー情報格納
		APlayerBase* Player = Cast<APlayerBase>(OtherActor);

		// カーブが終了したことをプレイヤーに知らせる
		Player->OnCurveMode(false);
	}
}

// Called every frame
void ACurveTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


