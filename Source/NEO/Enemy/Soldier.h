#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnamyBase.h"
#include "Soldier.generated.h"

UCLASS()
class NEO_API ASoldier : public AEnamyBase
{
    GENERATED_BODY()

public:
    ASoldier();
    UPROPERTY(EditAnywhere, Category = "AI")
        float MovementSpeed = 100.0f; // �ړ����x
    UPROPERTY(EditAnywhere, Category = "AI")
        float DesiredDistance = -500.0f;//�]�܂�������
protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;
    FVector GetPlayerDirection() const;
    float GetDistanceToPlayer() const;
public:
        bool bIsRotation;
    //�R�b���Ƃɓ����^�C�}�[
    FTimerHandle MoveStraightTimerHandle;

    
};