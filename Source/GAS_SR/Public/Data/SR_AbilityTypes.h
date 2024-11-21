#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "SR_AbilityTypes.generated.h"

class UGameplayEffect;
class USR_TargetType;

USTRUCT(BlueprintType)
struct FSR_GameplayEffectContainer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SR|GAS|GEContainer")
	TSubclassOf<USR_TargetType> TargetType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SR|GAS|GEContainer")
	TArray<TSubclassOf<UGameplayEffect>> TargetGameplayEffectsClasses;
};

USTRUCT(BlueprintType)
struct FSR_GameplayEffectContainerSpec
{
	GENERATED_USTRUCT_BODY()

	FSR_GameplayEffectContainerSpec() {}
	virtual ~FSR_GameplayEffectContainerSpec() = default;
	
	/* Property */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SR|GAS|GEContainerSpec")
	FGameplayAbilityTargetDataHandle TargetData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SR|GAS|GEContainer")
	TArray<FGameplayEffectSpecHandle> TargetGameplayEffectsSpecs;

	/* Function */
	virtual bool HasValidEffects() const
	{
		return TargetGameplayEffectsSpecs.Num() > 0;
	}

	virtual bool HasValidTargets() const
	{
		return TargetData.Num() > 0;
	}

	virtual void AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors)
	{
		for (const auto& Hit : HitResults)
		{
			const auto NewTargetData = new FGameplayAbilityTargetData_SingleTargetHit(Hit);
			TargetData.Add(NewTargetData);
		}

		if (TargetActors.Num() > 0)
		{
			const auto NewTargetData = new FGameplayAbilityTargetData_ActorArray();
			NewTargetData->TargetActorArray.Append(TargetActors);
			TargetData.Add(NewTargetData);
		}
	}
};