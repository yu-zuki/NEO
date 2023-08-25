// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "CurveTrigger.generated.h"

/**
 * 
 */
UCLASS()
class NEO_API ACurveTrigger : public ATriggerBox
{
	GENERATED_BODY()
	
public:

	// �R���X�g���N�^
	ACurveTrigger();

	// �Q�[���J�n���ɌĂяo����鏈��
	virtual void BeginPlay() override;

	//// ����Actor�ɏՓ˂����u�Ԃɍs������
	//virtual void OnActorOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	//// ����Actor�ɏՓ˂��I���u�Ԃɍs������
	//virtual void OnActorOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);


private:

	// ����p�̃{�b�N�X�R���|�[�l���g
	class UBoxComponent* CurveTriggerComp;
};
