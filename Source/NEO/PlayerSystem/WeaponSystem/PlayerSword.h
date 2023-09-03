// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "PlayerSword.generated.h"


UCLASS()
class NEO_API APlayerSword : public AWeaponBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerSword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
	// ìñÇΩÇËîªíË
	void SetCollision();

private:


	// ïêäÌÇÃìñÇΩÇËîªíË
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SwordCollision", meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* WeaponCollision;
};
