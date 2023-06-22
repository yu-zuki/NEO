// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAIController.h"
#include "UObject/ConstructorHelpers.h"

ABossAIController::ABossAIController(const class FObjectInitializer& ObjectInitializer)
{
	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BT_Boss"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BB_Boss"));

	// 作成したビヘイビアツリーを設定
	//ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Game/AI/BT_Enemy.BT_Enemy"));
	//BehaviorTree = BTFinder.Object;

	PlayerKeyName = "Player";
}

void ABossAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// AIControllerがPawn所持した際にBBとBTを使用
	BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	BehaviorComp->StartTree(*BehaviorTree);
}

void ABossAIController::OnUnPossess()
{
	Super::OnUnPossess();
	BehaviorComp->StopTree();
}

void ABossAIController::SetPlayerKey(APawn* player)
{
	ensure(BlackboardComp);

	// ブラックボードで作成したPlayerというキーにプレイヤー情報を入れる
	BlackboardComp->SetValueAsObject(PlayerKeyName, player);
}

ABossBase* ABossAIController::GetPlayerKey()
{
	ensure(BlackboardComp);

	return Cast<ABossBase>(BlackboardComp->GetValueAsObject(PlayerKeyName));
}