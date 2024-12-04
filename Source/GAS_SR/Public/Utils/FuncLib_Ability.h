#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Data/SR_AbilityTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuncLib_Ability.generated.h"

struct FActiveGameplayEffectHandle;

UCLASS()
class GAS_SR_API UFuncLib_Ability : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//应用自定义的GESpec
	UFUNCTION(BlueprintCallable, Category = "A_SR|FuncLib|Ability")
	static TArray<FActiveGameplayEffectHandle> ApplyEffectContainerSpec(const FSR_GameplayEffectContainerSpec& ContainerSpec);

	UFUNCTION(BlueprintCallable, Category = "A_SR|FuncLib|Ability", meta = (AutoCreateRefTerm = "HitResults,TargetActors"))
	static FSR_GameplayEffectContainerSpec AddTargetsToEffectContainerSpec(const FSR_GameplayEffectContainerSpec& ContainerSpec, const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);

	UFUNCTION(BlueprintCallable, Category = "A_SR|FuncLib|Ability")
	static FSR_GameplayEffectContainerSpec MakeEffectContainerExtra(AActor* OwnerActor, const FSR_GameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 Level);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "A_SR|FuncLib|Ability")
	static bool HaveEffects(const FSR_GameplayEffectContainerSpec& ContainerSpec);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "A_SR|FuncLib|Ability")
	static bool HaveTargets(const FSR_GameplayEffectContainerSpec& ContainerSpec);
};
