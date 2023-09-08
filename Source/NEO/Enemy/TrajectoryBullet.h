// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrajectoryBullet.generated.h"

UCLASS()
class NEO_API ATrajectoryBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrajectoryBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "Blinking")
		void StartBlinking();
    FTimerHandle& GetLifeSpanTimerHandle()
    {
        return LifeSpanTimerHandle;
    };
    float GetLifeSpan() const
    {
        return LifeSpan;
    }
private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
        class UStaticMeshComponent* BulletMeshComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blinking", meta = (AllowPrivateAccess = "true"))
        class UMaterialInterface* BulletMaterial;

    UPROPERTY()
        class UMaterialInstanceDynamic* DynamicMaterialInstance;

    float MinOpacity;
    float MaxOpacity;
    float BlinkDuration;
    FTimerHandle BlinkTimerHandle;
    FTimerHandle LifeSpanTimerHandle;
    float LifeSpan = 2.0f;
    void SetBulletOpacity(float Opacity);
    void BlinkBullet();
    void DeleteBullet();
};
