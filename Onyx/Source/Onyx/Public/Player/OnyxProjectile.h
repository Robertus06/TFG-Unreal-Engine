// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"

#include "OnyxProjectile.generated.h"

UCLASS()
class ONYX_API AOnyxProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOnyxProjectile();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability System")
	class UAbilitySystemComponent* OwnerAsc;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability System")
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true))
	float Range;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
