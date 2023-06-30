#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Soldier.generated.h"

UCLASS()
class NEO_API ASoldier : public AEnamyBase
{
    GENERATED_BODY()

public:
    ASoldier();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Movement")
        float Acceleration; // �����x

    UPROPERTY(EditAnywhere, Category = "Movement")
        float MaxSpeed; // �ő呬�x

    UPROPERTY(EditAnywhere, Category = "Movement")
        float DetectionRange; // ���m�͈�

    ACharacter* PlayerCharacter; // �v���C���[�L�����N�^�[�ւ̎Q��
    FVector TargetLocation; // �ڕW�ʒu
    bool bMovingForward; // �O�i���t���O

    float TimeSinceLastMovement; // �O��̓��삩��̌o�ߎ���

    void MoveForward(float Value);
    void DetectTarget();
    void SetNewTargetLocation();
    bool IsPlayerInDetectionRange();
    void StopMovement();
    void RotateTowardsPlayer();
};