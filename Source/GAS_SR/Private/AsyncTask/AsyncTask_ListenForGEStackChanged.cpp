#include "AsyncTask/AsyncTask_ListenForGEStackChanged.h"
#include "AbilitySystemComponent.h"

UAsyncTask_ListenForGEStackChanged* UAsyncTask_ListenForGEStackChanged::ListenForGEStackChanged(
	UObject* InWorldContextObject, UAbilitySystemComponent* InASComponent, FGameplayTag InEffectGameplayTag)
{
	AsyncTask = NewObject<UAsyncTask_ListenForGEStackChanged>();
	if (AsyncTask)
	{
		AsyncTask->WorldContextObject = InWorldContextObject;
		AsyncTask->ASComponent = InASComponent;
		AsyncTask->EffectGameplayTag = InEffectGameplayTag;
		return AsyncTask;
	}
	return nullptr;
}

void UAsyncTask_ListenForGEStackChanged::Activate()
{
	AddToRoot();
	RegisterWithGameInstance(WorldContextObject);

	if (!ASComponent || !EffectGameplayTag.IsValid())
	{
		EndTask();
		return;
	}

	ASComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ThisClass::OnActiveGameplayEffectAddedCallback);
	ASComponent->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ThisClass::OnRemoveGameplayEffectCallback);
}

void UAsyncTask_ListenForGEStackChanged::OnActiveGameplayEffectAddedCallback(
	UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(EffectGameplayTag) || GrantedTags.HasTagExact(EffectGameplayTag))
	{
		ASComponent->OnGameplayEffectStackChangeDelegate(ActiveHandle)->AddUObject(this, &ThisClass::GameplayEffectStackChanged);
		OnGameplayEffectStackChangeDelegate.Broadcast(EffectGameplayTag, ActiveHandle, 1, 0);
	}
}

void UAsyncTask_ListenForGEStackChanged::OnRemoveGameplayEffectCallback(const FActiveGameplayEffect& EffectRemoved)
{
	FGameplayTagContainer AssetTags;
	EffectRemoved.Spec.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	EffectRemoved.Spec.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(EffectGameplayTag) || GrantedTags.HasTagExact(EffectGameplayTag))
	{
		OnGameplayEffectStackChangeDelegate.Broadcast(EffectGameplayTag, EffectRemoved.Handle, 0, 1);
	}
}

void UAsyncTask_ListenForGEStackChanged::GameplayEffectStackChanged(FActiveGameplayEffectHandle EffectHandle, int32 NewStackCount, int32 PreviousStackCount)
{
	OnGameplayEffectStackChangeDelegate.Broadcast(EffectGameplayTag, EffectHandle, NewStackCount, PreviousStackCount);
}

void UAsyncTask_ListenForGEStackChanged::EndTask()
{
	if (IsValid(ASComponent))
	{
		ASComponent->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		ASComponent->OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);
	}
	SetReadyToDestroy();
	RemoveFromRoot();
	MarkPendingKill();
}
