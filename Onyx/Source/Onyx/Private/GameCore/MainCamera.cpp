// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/MainCamera.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMainCamera::AMainCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root->SetupAttachment(GetRootComponent());
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(Root);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = ArmLengthRange.Min;
}

// Called when the game starts or when spawned
void AMainCamera::BeginPlay()
{
	Super::BeginPlay();
	TargetLocation = GetActorLocation();
}

void AMainCamera::InitializeCamera()
{
	//get all player controllers
	const TArray<ULocalPlayer*>& LocalPlayers = GetGameInstance()->GetLocalPlayers();
	for (auto LocalPlayer : LocalPlayers)
	{
		TPlayerControllers.Add(LocalPlayer->GetPlayerController(GetWorld()));
	}
	UpdateTargetLocation();
}

// Called every frame
void AMainCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateCameraLocation(DeltaTime);
}

void AMainCamera::UpdateCameraLocation(float DeltaTime)
{
	if (TargetLocation.Equals(GetActorLocation(), 0.01f))
	{
		UpdateTargetLocation();
	}
	else
	{
		FVector pos(0.f);
		pos = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, CameraSpeed);
		SetActorLocation(pos);
	}
	if (FMath::IsNearlyEqual(SpringArm->TargetArmLength, SpringArmLength))
	{
		UpdateCameraArmLength();
	}
	else
	{
		float Value(0.f);
		Value = FMath::FInterpTo(SpringArm->TargetArmLength, SpringArmLength, DeltaTime, CameraSpeed);
		SpringArm->TargetArmLength = Value;
	}
}

void AMainCamera::UpdateTargetLocation()
{
	FVector sum(0.f);
	FVector avg(0.f);
	if (TPlayerControllers.Num() > 0)
	{
		for (int i = 0; i < TPlayerControllers.Num(); ++i)
		{
			if (const APawn* Pawn = TPlayerControllers[i]->GetPawn())
				sum += Pawn->GetActorLocation();
		}

		avg = sum / static_cast<float>(TPlayerControllers.Num());

		avg.Z = FixedZValue;

		TargetLocation = avg;
	}
}

void AMainCamera::UpdateCameraArmLength()
{
	if (TPlayerControllers.Num() > 0)
	{
		float Dist(0.0f);
		float MaxDist(0.0f);
		for (int i = 0; i < TPlayerControllers.Num(); ++i)
		{
			if (const APawn* Pawn = TPlayerControllers[i]->GetPawn())
			{
				Dist = EllipseDistance(Pawn->GetActorLocation(), TargetLocation);
				Dist > MaxDist ? MaxDist = Dist : 0;
			}
		}
		if (MaxDist > 0)
		{
			float Alpha(0.0f);
			if (MaxDist < PlayerDistanceRange.Min)
			{
				Alpha = 0.0f;
			}
			else if (MaxDist > PlayerDistanceRange.Max)
			{
				Alpha = 1.0f;
			}
			else
			{
				Alpha = (MaxDist - PlayerDistanceRange.Min) / (PlayerDistanceRange.Max - PlayerDistanceRange.Min);
			}
			SpringArmLength = ArmLengthRange.Interpolate(Alpha);
		}

	}
}

float AMainCamera::EllipseDistance(const FVector& A, const FVector& B) const
{
	const FVector Subs = A - B;
	const float Dist = Subs.X * Subs.X + Subs.Y * Subs.Y * DistanceRatio;
	return Dist;
}


