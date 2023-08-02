// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSpline.h"
#include "Components/SplineComponent.h"

// Sets default values
APlayerSpline::APlayerSpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add("PlayerLoad");

	SplineComp = CreateDefaultSubobject<USplineComponent>("PlayerLoad");

	//SplineComp->SetSplinePointType();
}

// Called when the game starts or when spawned
void APlayerSpline::BeginPlay()
{
	Super::BeginPlay();
	
	// �S�̂̋���
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
FRotator APlayerSpline::GetSplineAngle(float _distance)const
{
	if (SplineComp)
	{
		float m_localLength = FMath::Fmod(_distance,AllLength);

		//���݂̃X�v���C���̈ʒu�ɍ��킹����]���̒l���Q�ƂŕԂ�
		FRotator SplineAngle = SplineComp->GetRotationAtDistanceAlongSpline(m_localLength, ESplineCoordinateSpace::World);

		return SplineAngle;
	}

	return FRotator(0.0,0.0,0.0);
}


