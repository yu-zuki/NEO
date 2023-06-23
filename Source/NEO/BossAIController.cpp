// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAIController.h"
#include "UObject/ConstructorHelpers.h"

ABossAIController::ABossAIController(const class FObjectInitializer& ObjectInitializer)
{
	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	// �쐬�����r�w�C�r�A�c���[��ݒ�
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Game/0113/BOSS/BT_BossAI.BT_BossAI"));
	BehaviorTree = BTFinder.Object;

	PlayerKeyName = "Player";
	DistanceKeyName = "Distance";
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

void ABossAIController::SetToPlayerDistance(float* distance)
{
	ensure(BlackboardComp);

	// �u���b�N�{�[�h�ō쐬����Distance�Ƃ����L�[�Ƀv���C���[�Ƃ̋�����������
	BlackboardComp->SetValueAsFloat(PlayerKeyName, *distance);

	UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(*distance), true, true, FColor::Blue, 2.f);

}


ABossBase* ABossAIController::GetPlayerKey()
{
	ensure(BlackboardComp);

	return Cast<ABossBase>(BlackboardComp->GetValueAsObject(PlayerKeyName));
}

