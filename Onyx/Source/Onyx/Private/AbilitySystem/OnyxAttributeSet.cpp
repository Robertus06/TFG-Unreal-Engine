// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/OnyxAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "./Onyx/OnyxCharacter.h"
#include "GameCore/OnyxActor.h"

void UOnyxAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetMaxHealthAttribute())
	{
		ChangeAttributeMaxValue(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	if (Attribute == GetMaxManaAttribute())
	{
		ChangeAttributeMaxValue(Mana, MaxMana, NewValue, GetManaAttribute());
	}
	if (Attribute == GetMovementSpeedAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 150.f, 1200.f);
	}
}

void UOnyxAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if (Attribute == GetShieldAttribute())
	{
	}
}




void UOnyxAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	//Effect general info
	FGameplayEffectContextHandle ContextHandle = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* SourceASC = ContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	FGameplayTagContainer EffectTags;
	Data.EffectSpec.GetAllAssetTags(EffectTags);
	bool Critic = EffectTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Critic")));


	//Target Actor of effect (Attributes owner) info
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	AOnyxCharacter* TargetCharacter = nullptr;
	UAbilitySystemComponent* TargetASC = GetOwningAbilitySystemComponent();
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<AOnyxCharacter>(TargetActor);
	}


	//Handle Attributes changes
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}


	//Damage
	else if (Data.EvaluatedData.Attribute == GetPhysicalDamageAttribute())
	{
		bool Found;
		float PhysicalPenetration = SourceASC->GetGameplayAttributeValue(GetPhysicalPerforationAttribute(), Found);
		float PhysicalResistenceCoeficient = 100.f / (100.f + (GetArmor() * (1 - FMath::Clamp(PhysicalPenetration, 0.f, 1.f))));

		float DamageDone = GetPhysicalDamage() * PhysicalResistenceCoeficient;

		int DamageType = Critic ? 3 : 1;
		SetPhysicalDamage(0.f);

		if (DamageDone > 0.f)
		{
			float CurrentShield = GetShield();
			if (CurrentShield > 0)
			{
				if (DamageDone > CurrentShield)
				{
					//TargetASC->ApplyModToAttributeUnsafe(GetShieldAttribute(), EGameplayModOp::Additive, -CurrentShield);
					DamageDone -= CurrentShield;
					CurrentShield = 0.f;
				}
				else
				{
					//TargetASC->ApplyModToAttributeUnsafe(GetShieldAttribute(), EGameplayModOp::Additive, -DamageDone);
					CurrentShield -= DamageDone;
					DamageDone = 0.f;
				}
				GetShieldAttribute().SetNumericValueChecked(CurrentShield, this);
			}
			const float NewHealth = GetHealth() - DamageDone;

			if (AOnyxActor* Actor = Cast<AOnyxActor>(TargetActor))
			{
				Actor->LastHitter = SourceASC->AbilityActorInfo->AvatarActor.Get();
				if (DamageDone > 0)
					Actor->DamageEvent(DamageDone, DamageType);
			}
			else if (TargetCharacter && (TargetASC != SourceASC))
			{
				if (DamageDone > 0)
					TargetCharacter->DamageEvent(DamageDone, DamageType);
			}
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
		}
	}
	else if (Data.EvaluatedData.Attribute == GetMagicDamageAttribute())
	{
		bool Found;
		float MagicPenetration = SourceASC->GetGameplayAttributeValue(GetMagicPerforationAttribute(), Found);
		float MagicResistenceCoeficient = 100.f / (100.f + (GetMagicResistence() * (1 - FMath::Clamp(MagicPenetration, 0.f, 1.f))));
		float DamageDone = GetMagicDamage() * MagicResistenceCoeficient;
		int DamageType = Critic ? 3 : 2;
		SetMagicDamage(0.f);

		if (DamageDone > 0.f)
		{
			float CurrentShield = GetShield();
			if (CurrentShield > 0)
			{
				if (DamageDone > CurrentShield)
				{
					//TargetASC->ApplyModToAttributeUnsafe(GetShieldAttribute(), EGameplayModOp::Additive, -CurrentShield);
					DamageDone -= CurrentShield;
					CurrentShield = 0.f;
				}
				else
				{
					//TargetASC->ApplyModToAttributeUnsafe(GetShieldAttribute(), EGameplayModOp::Additive, -DamageDone);
					CurrentShield -= DamageDone;
					DamageDone = 0.f;
				}
				GetShieldAttribute().SetNumericValueChecked(CurrentShield, this);

			}
			const float NewHealth = GetHealth() - DamageDone;

			if (AOnyxActor* Actor = Cast<AOnyxActor>(TargetActor))
			{
				Actor->LastHitter = SourceASC->AbilityActorInfo->AvatarActor.Get();
				if (DamageDone > 0)
					Actor->DamageEvent(DamageDone, DamageType);
			}
			else if (TargetCharacter && (TargetASC != SourceASC))
			{
				if (DamageDone > 0)
					TargetCharacter->DamageEvent(DamageDone, DamageType);
			}

			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
		}
	}

}

void UOnyxAttributeSet::ChangeAttributeMaxValue(FGameplayAttributeData& BaseAttribute,
	FGameplayAttributeData& MaxAttribute, float NewMax, const FGameplayAttribute& BaseAttributeProperty)
{
	UAbilitySystemComponent* asc = GetOwningAbilitySystemComponent();
	const float CurrentMax = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMax, NewMax) && asc)
	{
		const float CurrentValue = BaseAttribute.GetCurrentValue();
		float Delta = (CurrentValue > 0.f) ? (CurrentValue * NewMax / CurrentMax) - CurrentValue : NewMax;

		asc->ApplyModToAttributeUnsafe(BaseAttributeProperty, EGameplayModOp::Additive, Delta);
	}
}
