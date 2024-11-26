#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AsyncTask_PlayMontageAndWaitEvent.generated.h"

class USR_AbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayMontageAndWaitEventSignature, FGameplayTag, EventTag, FGameplayEventData, EventData);

UCLASS()
class GAS_SR_API UAsyncTask_PlayMontageAndWaitEvent : public UAbilityTask
{
	GENERATED_BODY()

	/* Property */
private:
	// Delegate
	UPROPERTY(BlueprintAssignable, DisplayName="OnMontageCompleted")
	FPlayMontageAndWaitEventSignature OnMontageCompletedDelegate;
	
	UPROPERTY(BlueprintAssignable, DisplayName="OnMontageBlendOut")
	FPlayMontageAndWaitEventSignature OnMontageBlendOutDelegate;
	
	UPROPERTY(BlueprintAssignable, DisplayName="OnMontageInterrupted")
	FPlayMontageAndWaitEventSignature OnMontageInterruptedDelegate;
	
	UPROPERTY(BlueprintAssignable, DisplayName="OnMontageCancelled")
	FPlayMontageAndWaitEventSignature OnMontageCancelledDelegate;
	
	UPROPERTY(BlueprintAssignable, DisplayName="OnEventReceived")
	FPlayMontageAndWaitEventSignature OnEventReceivedDelegate;

	FOnMontageBlendingOutStarted OnBlendingOutStartedDelegate;
	FOnMontageEnded OnMontageEndedDelegate;
	FDelegateHandle CancelledHandle;
	FDelegateHandle EventHandle;

	// Variable
	UPROPERTY()
	UAnimMontage* MontageToPlay {};

	UPROPERTY()
	FGameplayTagContainer EventTags {};

	UPROPERTY()
	float Rate { 1.f };

	UPROPERTY()
	FName StartSection { NAME_None };

	UPROPERTY()
	float AnimRootMotionTranslationScale { 1.f };

	UPROPERTY()
	bool IsStopWhenAbilityEnd { true };
	
	/* Function */
public:
	UFUNCTION(BlueprintCallable, Category="SR|AsyncTask", DisplayName="PlayMontageAndWaitEvent_Async", meta=(DefaultToSelf="OwningAbility", HidePin="OwningAbility", BlueprintInternalUseOnly="true"))
	static UAsyncTask_PlayMontageAndWaitEvent* CreatePlayMontageAndWaitEventProxy(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		UAnimMontage* InMontageToPlay,
		FGameplayTagContainer InEventTags,
		float InRate = 1.f,
		FName InStartSection = NAME_None,
		bool bStopWhenAbilityEnd = true,
		float InAnimRootMotionTranslationScale = 1.f);

protected:
	virtual void Activate() override;
	virtual void ExternalCancel() override;
	virtual void OnDestroy(bool AbilityEnded) override;
	
	void OnMontageBlendingOutStarted(UAnimMontage* Montage, bool bInterrupted) const;
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload) const;
	void OnAbilityCancelled() const;

public:
	bool EndAbilityTask() const;
	USR_AbilitySystemComponent* GetTargetASC() const;
};
