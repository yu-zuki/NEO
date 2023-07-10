// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "PlayerBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/StreamableManager.h"
#include "Engine/World.h"
#include "Engine/AssetManager.h"
#include "Async/Async.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// プレイヤーの設定
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// キャラクターコンポーネント取得
	CharacterMovementComp = GetCharacterMovement();
	CharacterMovementComp->MaxWalkSpeed = 500.f;

	// アニメーションセットアップ
	TCHAR* AnimationAssetPaths[2];

	AnimationAssetPaths[0] = TEXT("Game/0122/Player/Animation/Montage/Combo/SwordCombo");
	AnimationAssetPaths[1] = TEXT("Game/0122/Player/Animation/Montage/Combo/SwordCombo2");


	SetupAnimationAsset(AnimationAssetPaths);

	// 武器のセットアップ

	// ボタン設定
	TCHAR* defaultMappingContext = TEXT("Game/0122/Player/Input/IMC_Default");

	// それぞれのアクション
	TArray<TCHAR*> inputAction;
	inputAction.Add(TEXT("Game/0122/Player/Input/Actions/IA_Move"));
	inputAction.Add(TEXT("Game/0122/Player/Input/Actions/IA_Dash"));
	inputAction.Add(TEXT("Game/0122/Player/Input/Actions/IA_Jump"));
	inputAction.Add(TEXT("Game/0122/Player/Input/Actions/IA_Combo1"));
	inputAction.Add(TEXT("Game/0122/Player/Input/Actions/IA_Combo2"));

	// セット
	SetupDefoultMappingContext(defaultMappingContext, inputAction);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	//if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	//{
	//	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	//	{
	//		Subsystem->AddMappingContext(DefaultMappingContext, 1);
	//	}
	//}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

