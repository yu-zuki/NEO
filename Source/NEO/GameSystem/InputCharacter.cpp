// Fill out your copyright notice in the Description page of Project Settings.


#include "InputCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TGS_GameMode.h"

// Sets default values
AInputCharacter::AInputCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInputCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			//第一引数はコンテキスト名、第二引数は優先度
			Subsystem->AddMappingContext(SubMappingContext, 0);
		}
	}
}

ATGS_GameMode* AInputCharacter::GetGamemode()
{
	ATGS_GameMode* gameMode = Cast<ATGS_GameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode) {
		return gameMode;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("GameMode is not found"));
		return nullptr;
	}
}

// Called every frame
void AInputCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AInputCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(UpAction, ETriggerEvent::Started, this, &AInputCharacter::ActionUp);
		EnhancedInputComponent->BindAction(DownAction, ETriggerEvent::Started, this, &AInputCharacter::ActionDown);
		EnhancedInputComponent->BindAction(LeftAction, ETriggerEvent::Started, this, &AInputCharacter::ActionLeft);
		EnhancedInputComponent->BindAction(RightAction, ETriggerEvent::Started, this, &AInputCharacter::ActionRight);
		EnhancedInputComponent->BindAction(EnterAction, ETriggerEvent::Started, this, &AInputCharacter::ActionEnter);
		EnhancedInputComponent->BindAction(BackAction, ETriggerEvent::Started, this, &AInputCharacter::ActionBack);
		EnhancedInputComponent->BindAction(MenuAction, ETriggerEvent::Started, this, &AInputCharacter::ActionMenu);
	}
}

void AInputCharacter:: ActionUp() { GetGamemode()->SetSubAction(ESubAction::ESubAction_Up); }
void AInputCharacter:: ActionDown() { GetGamemode()->SetSubAction(ESubAction::ESubAction_Down); }
void AInputCharacter:: ActionLeft() { GetGamemode()->SetSubAction(ESubAction::ESubAction_Left); }
void AInputCharacter:: ActionRight() { GetGamemode()->SetSubAction(ESubAction::ESubAction_Right); }
void AInputCharacter:: ActionEnter() { GetGamemode()->SetSubAction(ESubAction::ESubAction_Enter); }
void AInputCharacter:: ActionBack() { GetGamemode()->SetSubAction(ESubAction::ESubAction_Back); }
void AInputCharacter:: ActionMenu() { GetGamemode()->SetSubAction(ESubAction::ESubAction_Menu); }

