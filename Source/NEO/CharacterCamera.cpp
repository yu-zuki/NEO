// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterCamera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values
ACharacterCamera::ACharacterCamera()
	: PlayerToViewPointDistance_X(200.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bInheritPitch = false;
	CameraBoom->SetRelativeRotation(FRotator(-12.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	// Create a follow camera
	Follow_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Follow_Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	Follow_Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

// Called when the game starts or when spawned
void ACharacterCamera::BeginPlay()
{
	Super::BeginPlay();

	// APlayerController の取得
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (PlayerController)
	{
		PlayerController->SetViewTargetWithBlend(this);
	}
	PlayerInfo = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	PlayerInfo->SetOwner(this);

	// 初期位置設定
	StartPos = PlayerInfo->GetActorLocation();
	StartPos = FVector(StartPos.X - 150.f, StartPos.Y + PlayerToViewPointDistance_X, StartPos.Z + 100.f);
	SetActorLocation(StartPos);
}

// Called every frame
void ACharacterCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// プレイヤーの現在位置取得
	FVector PlayerPos = PlayerInfo->GetActorLocation();

	SetActorLocation(FVector(StartPos.X, PlayerPos.Y + PlayerToViewPointDistance_X,StartPos.Z));
}

