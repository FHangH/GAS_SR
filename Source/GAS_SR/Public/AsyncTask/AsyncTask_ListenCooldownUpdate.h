#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncTask_ListenCooldownUpdate.generated.h"

struct FGameplayEffectSpec;
struct FActiveGameplayEffectHandle;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCooldownChangedSignature, FGameplayTag, CooldownTag, float, TimeRemaining, float, CooldownDuration);

UCLASS(BlueprintType, meta=(ExposedAsyncProxy="AsyncTask"))
class GAS_SR_API UAsyncTask_ListenCooldownUpdate : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	/* Property */
private:
	static UAsyncTask_ListenCooldownUpdate* AsyncTask;
	static UObject* WorldContextObject;
	
	UPROPERTY()
	UAbilitySystemComponent* ASComponent {};
	FGameplayTagContainer CooldownTags {};
	bool IsUseServerCooldown { false };

	UPROPERTY(BlueprintAssignable, DisplayName="CooldownBegin")
	FOnCooldownChangedSignature OnCooldownBeginDelegate;

	UPROPERTY(BlueprintAssignable, DisplayName="CooldownEnd")
	FOnCooldownChangedSignature OnCooldownEndDelegate;
	
	/* Function */
public:
	UFUNCTION(BlueprintCallable, Category="SR|AsyncTask", DisplayName="ListenCooldownUpdateChanged_Async", meta=(HidePin="InWorldContextObject", DefaultToSelf="InWorldContextObject", BlueprintInternalUseOnly="true"))
	static UAsyncTask_ListenCooldownUpdate* ListenCooldownUpdateChanged(UObject* InWorldContextObject, UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer InCooldownTags, bool bUseServerCooldown);

protected:
	virtual void Activate() override;

	virtual void OnActiveGameplayEffectAddedCallBack(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);
	virtual void OnCooldownTagChanged(const FGameplayTag CooldownTag, int32 NewCount);
	bool GetCooldownRemainingForTag(FGameplayTagContainer InCooldownTags, float& TimeRemaining, float& CooldownDuration) const;
	
private:
	UFUNCTION(BlueprintCallable, Category="SR|AsyncTask")
	void EndTask();
};
UAsyncTask_ListenCooldownUpdate* UAsyncTask_ListenCooldownUpdate::AsyncTask = nullptr;
UObject* UAsyncTask_ListenCooldownUpdate::WorldContextObject = nullptr;