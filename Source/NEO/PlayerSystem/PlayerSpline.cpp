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
	
	// 全体の距離
	AllLength = SplineComp->GetSplineLength();

}

// Called every frame
void APlayerSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


/*
 * 関数名　　　　：GetSplineAngle()
 * 処理内容　　　：スプラインの角度参照
 * 引数１　　　　：float _distance・・・距離
 * 戻り値　　　　：その地点での角度
 */
FRotator APlayerSpline::GetSplineAngle(float _distance)const
{
	if (SplineComp)
	{
		float m_localLength = FMath::Fmod(_distance,AllLength);

		//現在のスプラインの位置に合わせた回転情報の値を参照で返す
		FRotator SplineAngle = SplineComp->GetRotationAtDistanceAlongSpline(m_localLength, ESplineCoordinateSpace::World);

		return SplineAngle;
	}

	return FRotator(0.0,0.0,0.0);
}


