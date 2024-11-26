#include "AsyncTask/AsyncTask_ListenCooldownUpdate.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"

UAsyncTask_ListenCooldownUpdate* UAsyncTask_ListenCooldownUpdate::ListenCooldownUpdateChanged(
	UObject* InWorldContextObject, UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTagContainer InCooldownTags, const bool bUseServerCooldown)
{
	AsyncTask = NewObject<UAsyncTask_ListenCooldownUpdate>();
	if (AsyncTask)
	{
		AsyncTask->WorldContextObject = InWorldContextObject;
		AsyncTask->ASComponent = AbilitySystemComponent;
		AsyncTask->CooldownTags = InCooldownTags;
		AsyncTask->IsUseServerCooldown = bUseServerCooldown;
		return AsyncTask;
	}
	return nullptr;
}

void UAsyncTask_ListenCooldownUpdate::Activate()
{
	AddToRoot();
	RegisterWithGameInstance(WorldContextObject);

	if (!ASComponent || CooldownTags.Num() < 1)
	{
		EndTask();
		return;
	}
	ASComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ThisClass::OnActiveGameplayEffectAddedCallBack);

	TArray<FGameplayTag> CooldownTagArray;
	CooldownTags.GetGameplayTagArray(CooldownTagArray);
	for (const auto& Tag : CooldownTagArray)
	{
		ASComponent->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnCooldownTagChanged);
	}
}

void UAsyncTask_ListenCooldownUpdate::OnActiveGameplayEffectAddedCallBack(
	UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	TArray<FGameplayTag> CooldownTagArray;
	CooldownTags.GetGameplayTagArray(CooldownTagArray);

	for (const auto& CooldownTag : CooldownTagArray)
	{
		if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
		{
			auto TimeRemaining = 0.f;
			auto CooldownDuration = 0.f;
			
			FGameplayTagContainer CooldownTagContainer {GrantedTags.GetByIndex(0)};
			GetCooldownRemainingForTag(CooldownTagContainer, TimeRemaining, CooldownDuration);

			if (ASComponent->GetOwnerRole() == ROLE_Authority)
			{
				OnCooldownBeginDelegate.Broadcast(CooldownTag, TimeRemaining, CooldownDuration);
			}
			else if (!IsUseServerCooldown && SpecApplied.GetContext().GetAbilityInstance_NotReplicated())
			{
				OnCooldownBeginDelegate.Broadcast(CooldownTag, TimeRemaining, CooldownDuration);
			}
			else if (IsUseServerCooldown && SpecApplied.GetContext().GetAbilityInstance_NotReplicated() == nullptr)
			{
				OnCooldownBeginDelegate.Broadcast(CooldownTag, TimeRemaining, CooldownDuration);
			}
			else if (IsUseServerCooldown && SpecApplied.GetContext().GetAbilityInstance_NotReplicated())
			{
				OnCooldownBeginDelegate.Broadcast(CooldownTag, -1.f, -1.f);
			}
		}
	}
}

void UAsyncTask_ListenCooldownUpdate::OnCooldownTagChanged(const FGameplayTag CooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		OnCooldownEndDelegate.Broadcast(CooldownTag, -1.f, -1.f);
	}
}

bool UAsyncTask_ListenCooldownUpdate::GetCooldownRemainingForTag(FGameplayTagContainer InCooldownTags, float& TimeRemaining, float& CooldownDuration) const
{
	if (ASComponent && InCooldownTags.Num() > 0)
	{
		TimeRemaining = 0.f;
		CooldownDuration = 0.f;

		const auto Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTags);
		auto DurationAndTimeRemaining = ASComponent->GetActiveEffectsTimeRemainingAndDuration(Query);

		if (DurationAndTimeRemaining.Num() > 0)
		{
			auto BestIdx = 0;
			auto LongestTime = DurationAndTimeRemaining[0].Key;
			for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
			{
				if (DurationAndTimeRemaining[Idx].Key > LongestTime)
				{
					LongestTime = DurationAndTimeRemaining[Idx].Key;
					BestIdx = Idx;
				}
			}
			TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
			CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;
			return true;
		}
	}
	return false;
}

void UAsyncTask_ListenCooldownUpdate::EndTask()
{
	if (ASComponent)
	{
		ASComponent->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		TArray<FGameplayTag> CooldownTagArray;
		CooldownTags.GetGameplayTagArray(CooldownTagArray);
		for (const auto& Tag : CooldownTagArray)
		{
			ASComponent->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		}
	}
	SetReadyToDestroy();
	RemoveFromRoot();
	MarkPendingKill();
}
