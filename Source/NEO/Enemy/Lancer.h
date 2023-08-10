#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/TargetPoint.h"
#include "Components/BoxComponent.h"
#include "EnamyBase.h"
#include "Lancer.generated.h"

UCLASS()
class NEO_API ALancer : public AEnamyBase
{
    GENERATED_BODY()

public:
    ALancer();
    UPROPERTY(EditAnywhere, Category = "AI")
        float DesiredDistance = 100.0f; // �v���C���[����̖]�܂��������i3m�j

    UPROPERTY(EditAnywhere, Category = "AI")
        float MovementSpeed = 100.0f; // �ړ����x
    float MoveSpeed = 100.0f;
    void ChooseNewTarget();
protected:
    virtual void BeginPlay() override;

    FVector GetSnappedDirection(const FVector& Direction) const;
    
    
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
    bool bIsRandMove;
    //FVector RoundDirectionT45Degrees(FVector direction) const;
    bool bShouldSkipNextMovement;
    /*float TimeUntilNextRandomMove = 0.0f;
    UPROPERTY(EditAnywhere, Category = "Movement")
        float MinWaitTime = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Movement")
        float MaxWaitTime = 5.0f;*/
    FTimerHandle MoveToTargetTimer;
   
    ATargetPoint* CurrentTarget = nullptr;
};
