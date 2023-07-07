// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Ingame_WG.generated.h"

/**
 * 
 */
UCLASS()
class NEO_API UIngame_WG : public UUserWidget
{
	GENERATED_BODY()

public:
	FName WidgetValue_EnemyName;										// Widget�ϐ��@�G�̖��O
	int32 WidgetValue_EnemyHP;											// �G��HP
	int32 WidgetValue_EnemyMaxHP;										// �G�̍ő�HP

    void SetWidgetValue(FName _EnemyName, int32 _NowHp, int32 _MaxHp);				// Widget�ϐ��@�G�̖��O���Z�b�g
};
