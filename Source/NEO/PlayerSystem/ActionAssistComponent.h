// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionAssistComponent.generated.h"


UCLASS(ClassGroup = (ActionAssist), meta = (BlueprintSpawnableComponent))
class NEO_API UActionAssistComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// �R���X�g���N�^
	UActionAssistComponent();

	// �q�b�g�X�g�b�v����
	void HitStop(float _speedDuringHitStop, float _stopTime);

	// �G�t�F�N�g�X�|�[��
	void SpawnEffect(class UNiagaraSystem* _hitParticle, FVector _spawnPos);

	// �I�[�i�[���J�����Ɍ�����
	void OwnerParallelToCamera(bool _lookRight);

	// �J�����V�F�C�N
	void CameraShake(TSubclassOf<UCameraShakeBase> _shakePattern, float _scale = 1.f);

	// �A�j���[�V�����Đ�
	void PlayAnimation(UAnimMontage* _toPlayAnimMontage, FName _startSectionName = "None", float _playRate = 1.f);

	// �T�E���h�Đ�
	void PlaySound(class USoundBase* _sound_Obj, float StartTime = 0);

	// ���C���΂�
	bool WallCheck(float _lineLength);

protected:

	// �Q�[���J�n���ɌĂяo����鏈��
	virtual void BeginPlay() override;

	// ���t���[���Ăяo����鏈��
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	// �q�b�g�X�g�b�v�I��
	void EndHitStop();


protected:

	//-------------�e�@�\��ON�EOFF---------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Assist")
		bool bUseCorrectAttackAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Assist")
		bool bUseHitStop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Assist")
		bool bUseHitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Assist")
		bool bUseFaceCamera;

	//----------------------------------------------------------------------------

private:

	// �^�C�}�[�n���h��
	FTimerHandle TimerHandle_HitStop;

	// �A�j���[�V�����v���C�����ǂ���
	bool IsAnimationPlaying;
};
