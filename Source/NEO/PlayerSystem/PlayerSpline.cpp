// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSpline.h"
#include "Components/SplineComponent.h"

// Sets default values
APlayerSpline::APlayerSpline()
{
 	// Tick()�𖈃t���[���Ăяo��������
	PrimaryActorTick.bCanEverTick = true;

	// "PlayerLoad"�^�O�ǉ�
	Tags.Add("PlayerLoad");

	// �X�v���C���R���|�[�l���g�쐬
	SplineComp = CreateDefaultSubobject<USplineComponent>("PlayerLoad");
}

// Called when the game starts or when spawned
void APlayerSpline::BeginPlay()
{
	Super::BeginPlay();
	
	// �S�̂̒����擾
	AllLength = SplineComp->GetSplineLength();
}

// Called every frame
void APlayerSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


/*
 * �֐����@�@�@�@�FGetSplineAngle()
 * �������e�@�@�@�F�X�v���C���̊p�x�Q��
 * �����P�@�@�@�@�Ffloat _distance�E�E�E����
 * �߂�l�@�@�@�@�F���̒n�_�ł̊p�x
 */
FRotator APlayerSpline::GetSplineAngle(float _distance)
{
	if (SplineComp)
	{
		// �v���C���[�̌��݂̈ړ�������%�Ŏ擾
		float m_localLength = (float)((int)_distance % (int)AllLength);/*FMath::Fmod(_distance,AllLength);*/

		//���݂̃X�v���C���̈ʒu�ɍ��킹����]���̒l���Q�ƂŕԂ�
		FRotator SplineAngle = SplineComp->GetRotationAtDistanceAlongSpline(m_localLength, ESplineCoordinateSpace::World);

		return SplineAngle;
	}

	return FRotator(0.0,0.0,0.0);
}


