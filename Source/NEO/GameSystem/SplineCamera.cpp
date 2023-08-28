// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/SplineComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "../PlayerSystem/PlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "TGS_GameMode.h"
#include "Components/BoxComponent.h"							//ボックスコンポーネントを使うため


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

	//Box
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));

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

	//CastTGSGameMode
	ATGS_GameMode* TGSGameMode = Cast<ATGS_GameMode>(UGameplayStatics::GetGameMode(this));
	if (TGSGameMode) {
		TGSGameMode->SetSplineCamera(this);
	}

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	//SetOwner
	if (PlayerCharacter) {
		PlayerCharacter->SetOwner(this);
	}
	
}

// Called every frame
void ASplineCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bMoveFlag != true) return;

	// プレイヤーを取得
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (PlayerCharacter && SplineComponent)
	{
		// プレイヤーの位置を取得
		FVector PlayerLocation = PlayerCharacter->GetActorLocation();

		// Spline曲線上でプレイヤーに最も近い点を取得
		FVector NearestPoint = SplineComponent->FindLocationClosestToWorldLocation(PlayerLocation, ESplineCoordinateSpace::World);

		
		UE_LOG(LogTemp, Log, TEXT("location: %s"), *NearestPoint.ToString());	

		//FVector CameraLocation = NearestPoint;	// プレイヤーとカメラの距離を取得

		FVector BeforeCameraLocation = CameraComponent->GetComponentLocation();
		FVector CameraLocation = FMath::VInterpTo(BeforeCameraLocation,NearestPoint,DeltaTime, CameraSpeed);	// プレイヤーとカメラの距離を取得


		DrawDebugPoint(GetWorld(), BeforeCameraLocation, 100, FColor(52, 220, 239), false);

		

		CameraComponent->SetWorldLocation(CameraLocation);

		// プレイヤーの方向を向く
		//FRotator NewRotation = (PlayerLocation - CameraLocation).Rotation();

		FRotator NewRotation = SplineComponent->FindRotationClosestToWorldLocation(CameraLocation, ESplineCoordinateSpace::World);
		NewRotation.Yaw += -90.f;
		NewRotation.Pitch = -25.f;

		CameraComponent->SetWorldRotation(NewRotation);
	}

	return;

}

