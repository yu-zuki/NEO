#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Lancer.generated.h"

UCLASS()
class NEO_API ALancer : public ACharacter
{
    GENERATED_BODY()

public:
    ALancer();
  UPROPERTY(EditAnywhere, Category = "AI")
        float DesiredDistance = 300.0f; // �v���C���[����̖]�܂��������i3m�j

    UPROPERTY(EditAnywhere, Category = "AI")
        float MovementSpeed = 300.0f; // �ړ����x
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
        UAnimMontage * Attack;
    UPROPERTY()
        class ACharacter* PlayerCharacter; // �v���C���[�L�����N�^�[�̎Q��
protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

private:
  

    FVector GetPlayerDirection() const;
    float GetDistanceToPlayer() const;
};
