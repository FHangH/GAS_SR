#include "Utils/FuncLib_Ability.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Data/Object/SR_TargetType.h"
#include "Gameplay/Character/SR_CharacterBase.h"
#include "Gameplay/GAS/SR_AbilitySystemComponent.h"

TArray<FActiveGameplayEffectHandle> UFuncLib_Ability::ApplyEffectContainerSpec(const FSR_GameplayEffectContainerSpec& ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> Out;
	for (const FGameplayEffectSpecHandle& Spec : ContainerSpec.TargetGameplayEffectsSpecs)
	{
		if (Spec.IsValid())
		{
			for (const TSharedPtr<FGameplayAbilityTargetData> Data : ContainerSpec.TargetData.Data)
			{
				Out.Append(Data->ApplyGameplayEffectSpec(*Spec.Data.Get()));
			}
		}
	}
	return Out;
}

FSR_GameplayEffectContainerSpec UFuncLib_Ability::AddTargetsToEffectContainerSpec(
	const FSR_GameplayEffectContainerSpec& ContainerSpec, const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors)
{
	auto NewSpec = ContainerSpec;
	NewSpec.AddTargets(HitResults, TargetActors);
	return NewSpec;
}

FSR_GameplayEffectContainerSpec UFuncLib_Ability::MakeEffectContainerExtra(
	AActor* OwnerActor, const FSR_GameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 Level)
{
	FSR_GameplayEffectContainerSpec ResultSpec;
	if (!OwnerActor)
	{
		return ResultSpec;
	}

	const auto OwningChar = Cast<ASR_CharacterBase>(OwnerActor);
	const auto OwningASC = Cast<USR_AbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor));

	if (OwningASC)
	{
		if (Container.TargetType.Get())
		{
			TArray<FHitResult> HitResults;
			TArray<AActor*> TargetActors;
			const auto TargetTypeCDO = Container.TargetType.GetDefaultObject();
			AActor* AvatarActor = OwnerActor;
		
			TargetTypeCDO->GetTargets(OwningChar, AvatarActor, EventData, HitResults, TargetActors);
			ResultSpec.AddTargets(HitResults, TargetActors);
		}

		if (Level == INDEX_NONE)
		{
			Level = 1;
		}

		for (const TSubclassOf<UGameplayEffect>& c : Container.TargetGameplayEffectsClasses)
		{
			ResultSpec.TargetGameplayEffectsSpecs.Add((OwningASC->MakeOutgoingSpec(c, Level, FGameplayEffectContextHandle())));
		}
	}
	return ResultSpec;
}

bool UFuncLib_Ability::HaveEffects(const FSR_GameplayEffectContainerSpec& ContainerSpec)
{
	return ContainerSpec.HasValidEffects();
}

bool UFuncLib_Ability::HaveTargets(const FSR_GameplayEffectContainerSpec& ContainerSpec)
{
	return ContainerSpec.HasValidTargets();
}
