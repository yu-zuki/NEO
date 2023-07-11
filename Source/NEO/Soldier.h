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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
        UAnimMontage* Attack;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
        UAnimMontage* Death;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
        UAnimMontage* Damage_Reaction;
    UPROPERTY()
        class ACharacter* PlayerCharacter; // プレイヤーキャラクターの参照
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
        float  Health;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
        float  MaxHealth;
    UFUNCTION(BlueprintCallable, Category = "Health")
        void ApplyDamage(float DamageAmount, float DeltaTime);
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
        float fJumpHeight;					//ジャンプ高さ

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
        float fGravityAcceleration;			//重力加速度

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
        float fJumpTime;					//ジャンプ時間

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
        bool bIsJumping;					//ジャンプ中かどうか

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
        FVector vJumpStartLocation;				//ジャンプ開始時の位置

    //ジャンプの開始
    UFUNCTION(BlueprintCallable, Category = "Jump")
        void StartJumpByGravity(float JumpHeight = 1500.f, float GravityAcceleration = 2000.f);

    //ジャンプ中
    UFUNCTION(BlueprintCallable, Category = "Jump")
        void JumpingByGravity(float DeltaTime);

    //ジャンプの終了
    UFUNCTION(BlueprintCallable, Category = "Jump")
        void EndJumpByGravity();

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
        float DelayBeforeAttack;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
        float AttackInterval;

    UFUNCTION()
        void StartAttack();

    UFUNCTION()
        void RepeatAttack();

    UPROPERTY(EditDefaultsOnly, Category = "AI")
        float MaxDistanceFromPlayer;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
        float MoveSpeed;

private:
    UPROPERTY(EditAnywhere, Category = "Rotation")
        bool bIsRotation;
    UFUNCTION()
        void MoveToPlayer();
    FVector PlayerLocation;
    FTimerHandle AttackTimerHandle;
};