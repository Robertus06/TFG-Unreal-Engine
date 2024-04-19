// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/OnyxProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AOnyxProjectile::AOnyxProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
}

// Called when the game starts or when spawned
void AOnyxProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOnyxProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

