#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncTask_ListenAttributeChanged.generated.h"

struct FOnAttributeChangeData;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChangedSignature, FGameplayAttribute, Attribute, float, NewValue, float, OldValue);

UCLASS(BlueprintType, meta=(ExposedAsyncProxy="AsyncTask"))
class GAS_SR_API UAsyncTask_ListenAttributeChanged : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	/* Property */
private:
	static UAsyncTask_ListenAttributeChanged* AsyncTask;
	static UObject* WorldContextObject;
	
	UPROPERTY()
	UAbilitySystemComponent* ASComponent;

	UPROPERTY()
	FGameplayAttribute AttributeToListen;

	UPROPERTY(BlueprintAssignable, DisplayName="AttributeChanged")
	FOnAttributeChangedSignature OnAttributeChangedDelegate;
	
	/* Function */
public:
	UFUNCTION(BlueprintCallable, Category="SR|AsyncTask", DisplayName="ListenAttributeChanged_Async", meta=(HidePin="WorldContext", DefaultToSelf="WorldContext", BlueprintInternalUseOnly="true"))
	static UAsyncTask_ListenAttributeChanged* ListenAttributeChanged(UObject* WorldContext, UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute Attribute);

protected:
	virtual void Activate() override;
	void OnAttributeValueChanged(const FOnAttributeChangeData& AttributeChangeData) const;

private:
	UFUNCTION(BlueprintCallable, Category="SR|AsyncTask")
	void EndTask();
};
UAsyncTask_ListenAttributeChanged* UAsyncTask_ListenAttributeChanged::AsyncTask = nullptr;
UObject* UAsyncTask_ListenAttributeChanged::WorldContextObject = nullptr;