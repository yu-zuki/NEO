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
        float DesiredDistance = 300.0f; // プレイヤーからの望ましい距離（3m）

    UPROPERTY(EditAnywhere, Category = "AI")
        float MovementSpeed = 300.0f; // 移動速度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
        UAnimMontage * Attack;
    UPROPERTY()
        class ACharacter* PlayerCharacter; // プレイヤーキャラクターの参照
protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

private:
  

    FVector GetPlayerDirection() const;
    float GetDistanceToPlayer() const;
};
