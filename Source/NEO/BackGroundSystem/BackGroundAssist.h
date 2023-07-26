// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BackGroundAssist.generated.h"


UCLASS( ClassGroup=(BackGroundAssist), meta=(BlueprintSpawnableComponent) )
class NEO_API UBackGroundAssist : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBackGroundAssist();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	// í‚ÉƒJƒƒ‰‚Ì•ûŒü‚ğŒü‚­ˆ—
	void ToFaceCamera();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Assist")
		bool bUseBillboard;


		
private:
	FVector BeforeCameraPos;
	FRotator BeforeCameraRot;
};
