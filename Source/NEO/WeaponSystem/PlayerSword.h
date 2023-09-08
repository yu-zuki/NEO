// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "PlayerSword.generated.h"


UCLASS()
class NEO_API APlayerSword : public AWeaponBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerSword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
	// �����蔻��
	void SetCollision();


private:

	// ��_���[�W���̃G�t�F�N�g
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
		class UNiagaraSystem* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
		class USoundBase* EnemyHitSoundObj;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
		class USoundBase* ObjectHitSoundObj;

	// ����̓����蔻��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SwordCollision", meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* WeaponCollision;

	// ���b�ԃq�b�g�X�g�b�v���N������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Action Assist")
		float HitStopTime = 0.2f;
};
