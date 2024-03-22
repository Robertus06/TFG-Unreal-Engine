// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "OnyxMMC_ManaCost.generated.h"

/**
 * 
 */
UCLASS()
class ONYX_API UOnyxMMC_ManaCost : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
};
