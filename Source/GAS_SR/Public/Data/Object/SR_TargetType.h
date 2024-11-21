#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "SR_TargetType.generated.h"

class ASR_CharacterBase;

UCLASS(BlueprintType, Blueprintable)
class GAS_SR_API USR_TargetType : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category="SR|Object")
	void GetTargets(ASR_CharacterBase* SourceCharacter, AActor* SourceActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};
