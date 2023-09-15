// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineCamera.generated.h"

UCLASS()
class NEO_API ASplineCamera : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASplineCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Create CameraSpringArm 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		class USpringArmComponent* CameraSpringArm;
	//Create Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		class UCameraComponent* CameraComponent;

	//Create Spline
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		class USplineComponent* SplineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		class UBoxComponent* BoxComponent;

public:
	//Camera Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float CameraSpeed;

	//Camera MoveFlag
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		bool bMoveFlag;

	//CurrentDistance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float CurrentDistance;

	float CurrentRatio;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
