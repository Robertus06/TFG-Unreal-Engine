// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/OnyxMMC_ManaCost.h"
#include "AbilitySystem/OnyxGameplayAbility.h"

float UOnyxMMC_ManaCost::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const UOnyxGameplayAbility* Ability = Cast<UOnyxGameplayAbility>(Spec.GetContext().GetAbilityInstance_NotReplicated());

	if (!Ability)
	{
		return 0.0f;
	}

	return Ability->ManaCost.GetValueAtLevel(Ability->GetAbilityLevel());
}
