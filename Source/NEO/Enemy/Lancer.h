#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "EnamyBase.h"
#include "EnemyDamage.h"
#include "Lancer.generated.h"

UCLASS()
class NEO_API ALancer : public AEnamyBase
{
    GENERATED_BODY()

public:
    ALancer();
    UPROPERTY(EditAnywhere, Category = "AI")
        float DesiredDistance = 100.0f; // プレイヤーからの望ましい距離（3m）

    UPROPERTY(EditAnywhere, Category = "AI")
        float MovementSpeed = 100.0f; // 移動速度
   
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
        AEnemyDamage* MyEnemyDamage;

    void EnableCollisionWhenMontagePlaying();

    void DisableCollisionWhenMontageNotPlaying();
protected:
    virtual void BeginPlay() override;
    
    
    virtual void Tick(float DeltaTime) override;
    
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    UPROPERTY(EditAnywhere, Category = "Rotation")
        bool bIsRotation;
    FVector GetPlayerDirection() const;
    float GetDistanceToPlayer() const;
    FTimerHandle TimerHandle_CheckPlayerInFront;
    bool bIsPlayerInFront;
    void CheckPlayerInFront();
   
};
