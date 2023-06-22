// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAIController.h"
#include "UObject/ConstructorHelpers.h"

ABossAIController::ABossAIController(const class FObjectInitializer& ObjectInitializer)
{
	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BT_Boss"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BB_Boss"));

	// �쐬�����r�w�C�r�A�c���[��ݒ�
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

	// AIController��Pawn���������ۂ�BB��BT���g�p
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

	// �u���b�N�{�[�h�ō쐬����Player�Ƃ����L�[�Ƀv���C���[��������
	BlackboardComp->SetValueAsObject(PlayerKeyName, player);
}

ABossBase* ABossAIController::GetPlayerKey()
{
	ensure(BlackboardComp);

	return Cast<ABossBase>(BlackboardComp->GetValueAsObject(PlayerKeyName));
}