#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncTask_ListenForGEStackChanged.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGameplayEffectStackChangedSignature, FGameplayTag, EffectGameplayTag, FActiveGameplayEffectHandle, Handle, int32, NewStackCount, int32, OldStackCount);

UCLASS(BlueprintType, meta=(ExposedAsyncProxy="AsyncTask"))
class GAS_SR_API UAsyncTask_ListenForGEStackChanged : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	/* Property */
private:
	static UAsyncTask_ListenForGEStackChanged* AsyncTask;
	static UObject* WorldContextObject;
	
	UPROPERTY()
	UAbilitySystemComponent* ASComponent;
	FGameplayTag EffectGameplayTag;

	UPROPERTY(BlueprintAssignable, DisplayName="OnGameplayEffectStackChange")
	FOnGameplayEffectStackChangedSignature OnGameplayEffectStackChangeDelegate;

	/* Function */
public:
	UFUNCTION(BlueprintCallable, Category="SR|AsyncTask", DisplayName="ListenForGEStackChanged_Async", meta=(HidePin="InWorldContextObject", DefaultToSelf="InWorldContextObject", BlueprintInternalUseOnly="true"))
	static UAsyncTask_ListenForGEStackChanged* ListenForGEStackChanged(UObject* InWorldContextObject, UAbilitySystemComponent* InASComponent, FGameplayTag InEffectGameplayTag);
	
protected:
	virtual void Activate() override;

	virtual void OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);
	virtual void OnRemoveGameplayEffectCallback(const FActiveGameplayEffect& EffectRemoved);
	virtual void GameplayEffectStackChanged(FActiveGameplayEffectHandle EffectHandle, int32 NewStackCount, int32 PreviousStackCount);
private:
	UFUNCTION(BlueprintCallable, Category="SR|AsyncTask")
	void EndTask();
};
UAsyncTask_ListenForGEStackChanged* UAsyncTask_ListenForGEStackChanged::AsyncTask = nullptr;
UObject* UAsyncTask_ListenForGEStackChanged::WorldContextObject = nullptr;