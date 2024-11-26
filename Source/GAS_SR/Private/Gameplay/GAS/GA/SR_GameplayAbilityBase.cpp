#include "GAS_SR/Public/Gameplay/GAS/GA/SR_GameplayAbilityBase.h"
#include "Data/Object/SR_TargetType.h"
#include "Gameplay/Character/SR_CharacterBase.h"
#include "Gameplay/GAS/SR_AbilitySystemComponent.h"

TArray<FActiveGameplayEffectHandle> USR_GameplayAbilityBase::ApplyGameplayEffectContainerSpec(const FSR_GameplayEffectContainerSpec& ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> OutActiveHandles;
	for (const auto& SpecHandle : ContainerSpec.TargetGameplayEffectsSpecs)
	{
		OutActiveHandles.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData));
	}
	return OutActiveHandles;
}

FSR_GameplayEffectContainerSpec USR_GameplayAbilityBase::MakeGameplayEffectContainerSpecFromContainer(
	const FSR_GameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	FSR_GameplayEffectContainerSpec OutEffectContainerSpec;
	
	const auto OwningActor = GetOwningActorFromActorInfo();
	if (!OwningActor) return OutEffectContainerSpec;

	const auto OwingCharacter = Cast<ASR_CharacterBase>(OwningActor);
	if (!OwingCharacter) return OutEffectContainerSpec;

	const auto OwingASC = Cast<USR_AbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (!OwingASC) return OutEffectContainerSpec;

	if (Container.TargetType.Get())
	{
		TArray<FHitResult> HitResults;
		TArray<AActor*> TargetActors;
		const auto TargetTypeCDO = Container.TargetType->GetDefaultObject<USR_TargetType>();
		const auto AvatarActor = GetAvatarActorFromActorInfo();
		TargetTypeCDO->GetTargets(OwingCharacter, AvatarActor, EventData, HitResults, TargetActors);
		OutEffectContainerSpec.AddTargets(HitResults, TargetActors);
	}
	if (OverrideGameplayLevel == INDEX_NONE)
	{
		OverrideGameplayLevel = GetAbilityLevel();
	}

	for (const auto& GEClass : Container.TargetGameplayEffectsClasses)
	{
		OutEffectContainerSpec.TargetGameplayEffectsSpecs.Add(MakeOutgoingGameplayEffectSpec(GEClass, OverrideGameplayLevel));
	}
	return OutEffectContainerSpec;
}

FSR_GameplayEffectContainerSpec USR_GameplayAbilityBase::MakeGameplayEffectContainerSpec(
	const FGameplayTag ContainerTag, const FGameplayEventData& EventData, const int32 OverrideGameplayLevel)
{
	const auto FoundContainer = Map_GameplayEffectContainer.Find(ContainerTag);
	if (FoundContainer)
	{
		return MakeGameplayEffectContainerSpecFromContainer(*FoundContainer, EventData, OverrideGameplayLevel);
	}
	return FSR_GameplayEffectContainerSpec{};
}

TArray<FActiveGameplayEffectHandle> USR_GameplayAbilityBase::ApplyGameplayEffectContainerAndApply(
	const FGameplayTag ContainerTag, const FGameplayEventData& EventData, const int32 OverrideGameplayLevel)
{
	return ApplyGameplayEffectContainerSpec(MakeGameplayEffectContainerSpec(ContainerTag, EventData, OverrideGameplayLevel));
}
