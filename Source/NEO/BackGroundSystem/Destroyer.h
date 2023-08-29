// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Destroyer.generated.h"

UCLASS()
class NEO_API ADestroyer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADestroyer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent*SkeletalMesh1;
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* SkeletalMesh2;
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* SkeletalMesh3;
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* SkeletalMesh4;
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* SkeletalMesh5;
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* SkeletalMesh6;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh1;
};
