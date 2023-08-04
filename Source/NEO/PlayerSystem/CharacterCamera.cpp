// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterCamera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "NEO/GameSystem/TGS_GameMode.h"

// Sets default values
ACharacterCamera::ACharacterCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bInheritPitch = false;
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

	ATGS_GameMode* GameMode = Cast<ATGS_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);

	if (PlayerController)
	{
		PlayerController->SetViewTargetWithBlend(this, 0.f);
	}

	/*if (GameMode)
	{
		GameMode->SetViewTargetWithBlend(this, 0.5f);
	}*/

	//PlayerInfo = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//PlayerInfo->SetOwner(this);
	PlayerInfo = GetPlayer();

	// 初期位置設定
	StartPos = PlayerInfo->GetActorLocation();
	PlayerToViewPointDistance = FVector(200.f,400.f,200.f);
	SetActorLocation(StartPos);
}

// Called every frame
void ACharacterCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACharacter* tmp_PlayerInfo = GetPlayer();
	if (!tmp_PlayerInfo) return;

	// プレイヤーの現在位置取得
	FVector PlayerPos = tmp_PlayerInfo->GetActorLocation();

	SetActorLocation(FVector(StartPos.X - PlayerToViewPointDistance.Z, PlayerPos.Y + PlayerToViewPointDistance.X,StartPos.Z + PlayerToViewPointDistance.Y));
}

ACharacter* ACharacterCamera::GetPlayer()
{
	ACharacter*  tmp_PlayerInfo = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if(tmp_PlayerInfo) tmp_PlayerInfo->SetOwner(this);
	return tmp_PlayerInfo;
}