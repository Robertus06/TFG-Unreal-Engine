// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include "OnyxCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class AOnyxCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()


	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;



protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	class UOnyxAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	const class UOnyxAttributeSet* OnyxAttributeSet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UGameplayEffect> DefaultAttributes;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayEffect>> InitialEffects;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class UGameplayAbility>> CharacterAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	int32 CharacterID;

public:
	AOnyxCharacter();

	// To add mapping context
	virtual void BeginPlay() override;


protected:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void InitializeAttributes();
	virtual void InitializeEffects();
	virtual void GiveAbilities();

	void OnHealthAttributeUpdate(const FOnAttributeChangeData& Data);
	void OnManaAttributeUpdate(const FOnAttributeChangeData& Data);
	void OnShieldAttributeUpdate(const FOnAttributeChangeData& Data);
	void OnOnyxAttributeUpdate(const FOnAttributeChangeData& Data);


	UFUNCTION(BlueprintImplementableEvent)
	void HealthChangedEvent(int32 Character, float Percentage);

	UFUNCTION(BlueprintImplementableEvent)
	void ManaChangedEvent(int32 Character, float Percentage);

	UFUNCTION(BlueprintImplementableEvent)
	void ShieldChangedEvent(int32 Character, float Percentage);

	UFUNCTION(BlueprintImplementableEvent)
	void OnyxChangedEvent(int32 Character, float Value);
};


