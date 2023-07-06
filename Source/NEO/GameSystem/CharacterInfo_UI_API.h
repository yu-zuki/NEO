// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterInfo_UI_API.generated.h"

/**
 * 
 */
UCLASS()
class NEO_API UCharacterInfo_UI_API : public UObject
{
	GENERATED_BODY()

public:
	virtual	 TTuple<string, int> GetCharacterInfo();	//First is character name, second is character Hp
};
