// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerSpline.generated.h"

UCLASS()
class NEO_API APlayerSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerSpline();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// スプラインの角度参照
	UFUNCTION(BlueprintCallable, Category = "Load")
		FRotator GetSplineAngle(float _distance)const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float AllLength;

private:
	class USplineComponent* SplineComp;
};
