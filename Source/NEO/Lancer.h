#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
        float MovementSpeed = 10.0f; // �ړ����x
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
        UAnimMontage * Attack;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
        UAnimMontage* Death;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
        UAnimMontage* Damage_Reaction;
    UPROPERTY()
        class ACharacter* PlayerCharacter; // �v���C���[�L�����N�^�[�̎Q��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
        float  Health;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
        float  MaxHealth;
    UFUNCTION(BlueprintCallable, Category = "Health")
        void ApplyDamage(float DamageAmount,float DeltaTime);
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
        float fJumpHeight;					//�W�����v����

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
        float fGravityAcceleration;			//�d�͉����x

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
        float fJumpTime;					//�W�����v����

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
        bool bIsJumping;					//�W�����v�����ǂ���

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
        FVector vJumpStartLocation;				//�W�����v�J�n���̈ʒu

    //�W�����v�̊J�n
    UFUNCTION(BlueprintCallable, Category = "Jump")
        void StartJumpByGravity(float JumpHeight = 1500.f, float GravityAcceleration = 2000.f);

    //�W�����v��
    UFUNCTION(BlueprintCallable, Category = "Jump")
        void JumpingByGravity(float DeltaTime);

    //�W�����v�̏I��
    UFUNCTION(BlueprintCallable, Category = "Jump")
        void EndJumpByGravity();
protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
    FVector GetPlayerDirection() const;
    float GetDistanceToPlayer() const;

};
