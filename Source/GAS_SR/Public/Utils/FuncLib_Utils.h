#pragma once

#include "CoreMinimal.h"
#include "Data/SR_Types.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "FuncLib_Utils.generated.h"

enum class ESR_InputMode : uint8;

UCLASS()
class GAS_SR_API UFuncLib_Utils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="A_SR|FuncLib|Controller")
	static void SetPlayerControllerInputMode(ESR_InputMode InputMode, APlayerController* PlayerController, const bool IsShowMouse);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "A_SR|FuncLib|Data")
	static FString DamageTypeToString(ESRDamageType Type);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "A_SR|FuncLib|Data")
	static ESRDamageType StringToDamageType(const FString& Name);

	template<typename TEnum>
	static FString SREnumToString(const FString& EnumName, TEnum Enum, const bool bShortName)
	{
		const UEnum* Ep = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
		if (Ep)
		{
			if (bShortName)
			{
				const FString TempStr = Ep->GetNameByValue(static_cast<int>(Enum)).ToString();
				TArray<FString> Arr = UKismetStringLibrary::ParseIntoArray(TempStr, TEXT("::"));
				if (Arr.Num() > 0)
				{
					return Arr.Last();
				}
			}
			return Ep->GetNameByValue(static_cast<int>(Enum)).ToString();
		}
		return TEXT("");
	}

	template<typename TEnum>
	static TEnum SRStringToEnum(const FString& EnumName, FString EnumValue)
	{
		const UEnum* Ep = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
		if (!Ep)
		{
			return TEnum(0);
		}
		return static_cast<TEnum>(Ep->GetIndexByName(FName(*FString(EnumValue))));
	}
};
