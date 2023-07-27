// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPPOdaEnum.generated.h"


UENUM(BlueprintType)
enum class ECPPOdaEnum : uint8
{
	Stay1,
	Move1,
	Move2,
	Back1,
	Attack1,
	Attack2,
	Ultimate
};