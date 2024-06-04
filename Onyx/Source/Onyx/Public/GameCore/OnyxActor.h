// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "OnyxActor.generated.h"

UCLASS()
class ONYX_API AOnyxActor : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOnyxActor();

	UFUNCTION(BlueprintImplementableEvent)
	void DamageEvent(float Number, int Type);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void InitializeAttributes();
	virtual void InitializeEffects();

	void OnHealthAttributeUpdate(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintImplementableEvent)
	void HealthChangedEvent(float Percentage);

	UFUNCTION(BlueprintImplementableEvent)
	void DeadEvent(AActor* EffectInstigator);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY()
	AActor* LastHitter;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	class UOnyxAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	const class UOnyxAttributeSet* OnyxAttributeSet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UGameplayEffect> DefaultAttributes;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayEffect>> InitialEffects;

};
