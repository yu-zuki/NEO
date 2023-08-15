// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerSpline.generated.h"

UCLASS()
class NEO_API APlayerSpline : public AActor
{
	GENERATED_BODY()
	
private:

	// �R���X�g���N�^
	APlayerSpline();

	// �Q�[���J�n���ɌĂяo����鏈��
	virtual void BeginPlay() override;

	// ���t���[���Ăяo����鏈��
	virtual void Tick(float DeltaTime) override;

public:

	// �X�v���C���̊p�x�Q��
	FRotator GetSplineAngle(float _distance);

private:

	// �X�v���C���R���|�[�l���g
	class USplineComponent* SplineComp;

	// �X�v���C���S�̂̒���
	float AllLength;
};
