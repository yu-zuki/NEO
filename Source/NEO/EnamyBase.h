// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnamyBase.generated.h"

UCLASS()
class NEO_API AEnamyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnamyBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Area")
	bool IsAreaEnemy = false;
	//�A�j���[�V����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* Death;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* Damage_Reaction;
	UFUNCTION(BlueprintCallable, Category = "Area")
	virtual void DestoryEnemy();
	UPROPERTY()
		class ACharacter* PlayerCharacter; // �v���C���[�L�����N�^�[�̎Q��
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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float  Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float  MaxHealth;
	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual void ApplyDamage(float DamageAmount, float DeltaTime);

	float GetHP() { return Health; }

	//UserWidget class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UUserWidget> EnemyWidgetClass;

	//UserWidget Component 
	class UWidgetComponent* EnemyWidget;

	//�J�����Ɍ����̐ݒ�
	void SetFaceCamera();
};
