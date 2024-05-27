// Copyright Epic Games, Inc. All Rights Reserved.

#include "OnyxCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "OnyxGameMode.h"
#include "Onyx/Public/AbilitySystem/OnyxAbilitySystemComponent.h"
#include "Onyx/Public/AbilitySystem/OnyxAttributeSet.h"
#include "Abilities/GameplayAbility.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AOnyxCharacter

AOnyxCharacter::AOnyxCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;


	AbilitySystemComponent = CreateDefaultSubobject<UOnyxAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	OnyxAttributeSet = CreateDefaultSubobject<UOnyxAttributeSet>(TEXT("AttributeSet"));



	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AOnyxCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (IsValid(AbilitySystemComponent))
	{
		OnyxAttributeSet = AbilitySystemComponent->GetSet<UOnyxAttributeSet>();

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(OnyxAttributeSet->GetHealthAttribute()).AddUObject(this, &AOnyxCharacter::OnHealthAttributeUpdate);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(OnyxAttributeSet->GetManaAttribute()).AddUObject(this, &AOnyxCharacter::OnManaAttributeUpdate);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(OnyxAttributeSet->GetShieldAttribute()).AddUObject(this, &AOnyxCharacter::OnShieldAttributeUpdate);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(OnyxAttributeSet->GetOnyxShardsAttribute()).AddUObject(this, &AOnyxCharacter::OnOnyxAttributeUpdate);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(OnyxAttributeSet->GetMovementSpeedAttribute()).AddUObject(this, &AOnyxCharacter::OnMovementSpeedAttributeUpdate);
	}

	InitializeAttributes();
	InitializeEffects();
}

UAbilitySystemComponent* AOnyxCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AOnyxCharacter::InitializeAttributes()
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

void AOnyxCharacter::InitializeEffects()
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

void AOnyxCharacter::GiveAbilities()
{
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}

	for (TSubclassOf<UGameplayAbility>& StartAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartAbility, 1, INDEX_NONE, this));
	}
}

void AOnyxCharacter::OnHealthAttributeUpdate(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue > 0)
	{
		HealthChangedEvent(CharacterID, Data.NewValue / OnyxAttributeSet->GetMaxHealth());
	}
	else if(Data.NewValue <= 0)
	{
		HealthChangedEvent(CharacterID, 0.f);
		Dead();
	}
}

void AOnyxCharacter::OnManaAttributeUpdate(const FOnAttributeChangeData& Data)
{
	ManaChangedEvent(CharacterID, Data.NewValue / OnyxAttributeSet->GetMaxMana());
}

void AOnyxCharacter::OnShieldAttributeUpdate(const FOnAttributeChangeData& Data)
{
	ShieldChangedEvent(CharacterID, Data.NewValue / OnyxAttributeSet->GetMaxHealth());
}

void AOnyxCharacter::OnOnyxAttributeUpdate(const FOnAttributeChangeData& Data)
{
	OnyxChangedEvent(CharacterID, Data.NewValue);
}

void AOnyxCharacter::OnMovementSpeedAttributeUpdate(const FOnAttributeChangeData& Data)
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed = Data.NewValue;
	}
}

void AOnyxCharacter::Dead()
{
	PlayAnimMontage(DeadAnimation);
	if(const auto PC = GetLocalViewingPlayerController())
	{
		DisableInput(PC);
	}
	AOnyxGameMode* GameMode = GetWorld()->GetAuthGameMode<AOnyxGameMode>();
	if(GameMode)
	{
		GameMode->PlayerDead(CharacterID);
	}
}


#pragma region Inputs
//////////////////////////////////////////////////////////////////////////
// Input

//void AOnyxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	// Set up action bindings
//	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
//		
//		// Jumping
//		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
//		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
//
//		// Moving
//		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOnyxCharacter::Move);
//
//		// Looking
//		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AOnyxCharacter::Look);
//	}
//	else
//	{
//		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
//	}
//}
//
//void AOnyxCharacter::Move(const FInputActionValue& Value)
//{
//	// input is a Vector2D
//	FVector2D MovementVector = Value.Get<FVector2D>();
//
//	if (Controller != nullptr)
//	{
//		// find out which way is forward
//		const FRotator Rotation = Controller->GetControlRotation();
//		const FRotator YawRotation(0, Rotation.Yaw, 0);
//
//		// get forward vector
//		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
//	
//		// get right vector 
//		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
//
//		// add movement 
//		AddMovementInput(ForwardDirection, MovementVector.Y);
//		AddMovementInput(RightDirection, MovementVector.X);
//	}
//}
//
//void AOnyxCharacter::Look(const FInputActionValue& Value)
//{
//	// input is a Vector2D
//	FVector2D LookAxisVector = Value.Get<FVector2D>();
//
//	if (Controller != nullptr)
//	{
//		// add yaw and pitch input to controller
//		AddControllerYawInput(LookAxisVector.X);
//		AddControllerPitchInput(LookAxisVector.Y);
//	}
//}
#pragma endregion
