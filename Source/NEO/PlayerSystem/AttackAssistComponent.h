// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackAssistComponent.generated.h"


UCLASS( ClassGroup=(ActionAssist), meta=(BlueprintSpawnableComponent) )
class NEO_API UAttackAssistComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttackAssistComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// �U���̊p�x�C��
	void CorrectAttackAngle();

	// �q�b�g�X�g�b�v����
	void HitStop();

	// �G�t�F�N�g�X�|�[��
	void SpawnHitEffect();

	// �I�[�i�[���J�����Ɍ�����
	void OwnerParallelToCamera(bool _lookRight);

private:

	// �O���ɂ���Actor��Ԃ�
	AActor* GetFrontActor();

	// �q�b�g�X�g�b�v�I��
	void EndHitStop();


protected:

	//-------------�e�@�\��ON�EOFF---------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Assist")
		bool bUseCorrectAttackAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Assist")
		bool bUseHitStop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Assist")
		bool bUseHitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Assist")
		bool bUseFaceCamera;

	//----------------------------------------------------------------------------

	// �g���[�X�̒���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Assist")
		float LineLength;
		
	// HitStop�Ŏ~�܂鎞��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Assist")
		float SpeedDuringHitStop;

	// HitStop�Ŏ~�܂鎞��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Assist")
		float HitStopTime;

private:
	FTimerHandle TimerHandle_HitStop;
};
