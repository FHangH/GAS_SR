#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ESR_AbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	AbilityMain,
	AbilitySecondary,
	Ability1,
	Ability2,
};

UENUM(BlueprintType)
enum class ESR_InputMode : uint8
{
	OnlyUI,
	OnlyGame,
	Both
};