#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuncLib_Utils.generated.h"

enum class ESR_InputMode : uint8;

UCLASS()
class GAS_SR_API UFuncLib_Utils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="SR|FuncLib")
	static void SetPlayerControllerInputMode(ESR_InputMode InputMode, APlayerController* PlayerController, const bool IsShowMouse);
};
