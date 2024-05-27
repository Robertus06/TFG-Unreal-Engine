// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OnyxGameMode.generated.h"

UCLASS(minimalapi)
class AOnyxGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOnyxGameMode();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayerDead(int PlayerID);
};



