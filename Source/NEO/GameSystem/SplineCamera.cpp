// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/SplineComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "../PlayerSystem/PlayerCharacter.h"
#include "TGS_GameMode.h"

// Sets default values
ASplineCamera::ASplineCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraSpringArm);

	//CreateSplineComponent
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));

	bMoveFlag = false;
	CameraSpeed = 300.0f;

}

// Called when the game starts or when spawned
void ASplineCamera::BeginPlay()
{
	Super::BeginPlay();

	if (SplineComponent) {
		FVector CameraLocation = SplineComponent->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World);

		CameraComponent->SetWorldLocation(CameraLocation);
	}

	//SetViewTarget
	ATGS_GameMode* GameMode = Cast<ATGS_GameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode) {
		GameMode->SetViewTargetWithBlend(this);
	}
	
}

// Called every frame
void ASplineCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 获取玩家角色
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (PlayerCharacter && SplineComponent)
	{
		// 获取玩家角色的位置
		FVector PlayerLocation = PlayerCharacter->GetActorLocation();

		// 获取Spline曲线上距离玩家最近的点
		FVector NearestPoint = SplineComponent->FindLocationClosestToWorldLocation(PlayerLocation, ESplineCoordinateSpace::World);

		// 设置相机位置为Spline曲线上距离玩家最近的点
		FVector CameraLocation = NearestPoint;
		CameraComponent->SetWorldLocation(CameraLocation);

		// 使相机朝向玩家
		FRotator NewRotation = (PlayerLocation - CameraLocation).Rotation();

		CameraComponent->SetWorldRotation(NewRotation);
	}

	return;

}

