// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PlayerCharacter.h"
#include "BossBase.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class NEO_API ABossAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABossAIController(const class FObjectInitializer& ObjectInitializer);

public:
	void SetPlayerKey(APawn* player);

	void SetToPlayerDistance(float* distance);

	UFUNCTION()
		ABossBase* GetPlayerKey();


	UPROPERTY()
		UBehaviorTreeComponent* BehaviorComp;

	UPROPERTY()
		UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditDefaultsOnly, Category = AI)
		FName PlayerKeyName;	
	
	UPROPERTY(EditDefaultsOnly, Category = AI)
		FName DistanceKeyName;

protected:
	// AIControllerÇÃPawnèäéù
	virtual void OnPossess(class APawn* InPawn) override;

	// AIControllerÇÃPawnèäéùâèú
	virtual void OnUnPossess() override;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = AI)
		class UBehaviorTree* BehaviorTree;

	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
};