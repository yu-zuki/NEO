// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player_UMG.generated.h"

/**
 * 
 */
UCLASS()
class NEO_API UPlayer_UMG : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Meta = (BindWidget))
		class UCanvasPanel* Canvas;

	//HPÉoÅ[
	UPROPERTY(Meta = (BindWidget))
		class UProgressBar* HealthBar;

	//Skill âÊëú
	UPROPERTY(Meta = (BindWidget))
		class UImage* SkillImage;
};
