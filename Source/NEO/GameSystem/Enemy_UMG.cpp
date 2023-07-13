// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_UMG.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UEnemy_UMG::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEnemy_UMG::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (HealthBar)	{
		HealthBar->SetPercent(GetHPPercent());
	}

	if(Text){
		Text->SetText(FText::FromName(EnemyName));
	}
}
