#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(SRLog,Log,All);

#define SRLOG(lg) UE_LOG(SRLog,Log,lg);
#define SRWARNING(lg) UE_LOG(SRLog,Warning,lg)
#define SRWARNING1(lg,ex1) UE_LOG(SRLog,Warning,lg,ex1)
#define SRLOGEX1(lg,ex1) UE_LOG(SRLog,Log,lg,ex1)
#define SRLOGEX2(lg,ex1,ex2) UE_LOG(SRLog,Log,lg,ex1,ex2)
#define SRLOGEX3(lg,ex1,ex2,ex3) UE_LOG(SRLog,Log,lg,ex1,ex2,ex3)
#define SRLOGEX4(lg,ex1,ex2,ex3,ex4) UE_LOG(SRLog,Log,lg,ex1,ex2,ex3,ex4)
#define SRLOGEX5(lg,ex1,ex2,ex3,ex4,ex5) UE_LOG(SRLog,Log,lg,ex1,ex2,ex3,ex4,ex5)

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