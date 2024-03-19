// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "OnyxAttributeSet.generated.h"

/**
 *
 */

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class ONYX_API UOnyxAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UOnyxAttributeSet, Health)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UOnyxAttributeSet, HealthRegeneration)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UOnyxAttributeSet, Armor)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MagicResistence;
	ATTRIBUTE_ACCESSORS(UOnyxAttributeSet, MagicResistence)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MagicPower;
	ATTRIBUTE_ACCESSORS(UOnyxAttributeSet, MagicPower)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData PhysicalPower;
	ATTRIBUTE_ACCESSORS(UOnyxAttributeSet, PhysicalPower)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MagicPerforation;
	ATTRIBUTE_ACCESSORS(UOnyxAttributeSet, MagicPerforation)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData PhysicalPerforation;
	ATTRIBUTE_ACCESSORS(UOnyxAttributeSet, PhysicalPerforation)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData HealthSteal;
	ATTRIBUTE_ACCESSORS(UOnyxAttributeSet, HealthSteal)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData CriticChance;
	ATTRIBUTE_ACCESSORS(UOnyxAttributeSet, CriticChance)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData CriticDamage;
	ATTRIBUTE_ACCESSORS(UOnyxAttributeSet, CriticDamage)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UOnyxAttributeSet, MovementSpeed)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData CooldownReduction;
	ATTRIBUTE_ACCESSORS(UOnyxAttributeSet, CooldownReduction)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UOnyxAttributeSet, Mana)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UOnyxAttributeSet, ManaRegeneration)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData AttackRange;
	ATTRIBUTE_ACCESSORS(UOnyxAttributeSet, AttackRange)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData OnyxShards;
	ATTRIBUTE_ACCESSORS(UOnyxAttributeSet, OnyxShards)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UOnyxAttributeSet, Shield)

	//	LIMIT ATTRIBUTES
		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UOnyxAttributeSet, MaxHealth)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UOnyxAttributeSet, MaxMana)

	//	META ATTRIBUTES
		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData PhysicalDamage;
	ATTRIBUTE_ACCESSORS(UOnyxAttributeSet, PhysicalDamage)

		UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MagicDamage;
	ATTRIBUTE_ACCESSORS(UOnyxAttributeSet, MagicDamage)

protected:
	void ChangeAttributeMaxValue(FGameplayAttributeData& BaseAttribute, FGameplayAttributeData& MaxAttribute, float NewMax, const FGameplayAttribute& BaseAttributeProperty);
};
