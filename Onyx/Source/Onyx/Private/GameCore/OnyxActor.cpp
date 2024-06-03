// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/OnyxActor.h"

#include "GameplayEffectExtension.h"
#include "AbilitySystem/OnyxAbilitySystemComponent.h"
#include "AbilitySystem/OnyxAttributeSet.h"

// Sets default values
AOnyxActor::AOnyxActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UOnyxAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	OnyxAttributeSet = CreateDefaultSubobject<UOnyxAttributeSet>(TEXT("AttributeSet"));

}

// Called when the game starts or when spawned
void AOnyxActor::BeginPlay()
{
	Super::BeginPlay();
	if(IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(OnyxAttributeSet->GetHealthAttribute()).AddUObject(this, &AOnyxActor::OnHealthAttributeUpdate);

	}
	InitializeAttributes();
	InitializeEffects();
}

UAbilitySystemComponent* AOnyxActor::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AOnyxActor::InitializeAttributes()
{
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	AbilitySystemComponent->ApplyGameplayEffectToSelf(DefaultAttributes.GetDefaultObject(), 0, EffectContext);
}

void AOnyxActor::InitializeEffects()
{
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}

	for (auto Effect : InitialEffects)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		AbilitySystemComponent->ApplyGameplayEffectToSelf(Effect.GetDefaultObject(), 0, EffectContext);
	}
}

// Called every frame
void AOnyxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOnyxActor::OnHealthAttributeUpdate(const FOnAttributeChangeData& Data)
{
	HealthChangedEvent(Data.NewValue / OnyxAttributeSet->GetMaxHealth());

	if(Data.NewValue <= 0.0f)
	{
		if(LastHitter)
			DeadEvent(LastHitter);

		//Remove all active effects (Healing, mana regen ...)
		FGameplayTagContainer TempTag;
		TempTag.AddTag(FGameplayTag::RequestGameplayTag(FName("State")));
		AbilitySystemComponent->RemoveActiveEffectsWithAppliedTags(TempTag);
	}

}

