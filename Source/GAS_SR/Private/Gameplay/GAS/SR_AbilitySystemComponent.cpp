#include "GAS_SR/Public/Gameplay/GAS/SR_AbilitySystemComponent.h"
#include "GAS_SR/Public/Gameplay/GAS/GA/SR_GameplayAbilityBase.h"

USR_AbilitySystemComponent::USR_AbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USR_AbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USR_AbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool USR_AbilitySystemComponent::AddNewAbility(const TSubclassOf<USR_GameplayAbilityBase> AbilityClass, const int32 Level)
{
	if (!AbilityClass) return false;
	if (AbilityClass && Map_AbilitySpec.Num() > 0)
	{
		for (const auto& AbilitySpec : Map_AbilitySpec)
		{
			if (AbilitySpec.Value->Ability->GetClass() == AbilityClass) return false;
		}
	}

	const auto AbilitySpec = FGameplayAbilitySpec
	{
		AbilityClass, Level, static_cast<int32>(AbilityClass.GetDefaultObject()->AbilityInputID), GetOwnerActor()
	};
	GiveAbility(AbilitySpec);
	return true;
}