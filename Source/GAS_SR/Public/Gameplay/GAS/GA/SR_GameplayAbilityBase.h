#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Data/SR_AbilityTypes.h"
#include "Data/SR_Types.h"
#include "SR_GameplayAbilityBase.generated.h"

UCLASS(BlueprintType)
class GAS_SR_API USR_GameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="A_SR")
	FString AbilityName {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="A_SR")
	ESR_AbilityInputID AbilityInputID { ESR_AbilityInputID::None };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="A_SR")
	bool IsAutoActivate { false };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="A_SR")
	TMap<FGameplayTag, FSR_GameplayEffectContainer> Map_GameplayEffectContainer;

	/* Function */
public:
	UFUNCTION(BlueprintCallable, Category="A_SR|Ability")
	virtual TArray<FActiveGameplayEffectHandle> ApplyGameplayEffectContainerSpec(const FSR_GameplayEffectContainerSpec& ContainerSpec);

	UFUNCTION(BlueprintCallable, Category="A_SR|Ability", meta=(AutoCreateRefTerm="EventData"))
	virtual FSR_GameplayEffectContainerSpec MakeGameplayEffectContainerSpecFromContainer(const FSR_GameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	UFUNCTION(BlueprintCallable, Category="A_SR|Ability", meta=(AutoCreateRefTerm="EventData"))
	virtual FSR_GameplayEffectContainerSpec MakeGameplayEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	UFUNCTION(BlueprintCallable, Category="A_SR|Ability", meta=(AutoCreateRefTerm="EventData"))
	virtual TArray<FActiveGameplayEffectHandle> ApplyGameplayEffectContainerAndApply(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);
};