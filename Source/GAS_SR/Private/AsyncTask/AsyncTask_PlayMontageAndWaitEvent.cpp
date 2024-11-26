#include "AsyncTask/AsyncTask_PlayMontageAndWaitEvent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"
#include "Gameplay/GAS/SR_AbilitySystemComponent.h"

UAsyncTask_PlayMontageAndWaitEvent* UAsyncTask_PlayMontageAndWaitEvent::CreatePlayMontageAndWaitEventProxy(
	UGameplayAbility* OwningAbility, const FName TaskInstanceName, UAnimMontage* InMontageToPlay,
	const FGameplayTagContainer InEventTags, float InRate, const FName InStartSection, const bool bStopWhenAbilityEnd,
	const float InAnimRootMotionTranslationScale)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(InRate);

	const auto AsyncTaskObject = NewAbilityTask<UAsyncTask_PlayMontageAndWaitEvent>(OwningAbility, TaskInstanceName);
	if (AsyncTaskObject)
	{
		AsyncTaskObject->MontageToPlay = InMontageToPlay;
		AsyncTaskObject->EventTags = InEventTags;
		AsyncTaskObject->Rate = InRate;
		AsyncTaskObject->StartSection = InStartSection;
		AsyncTaskObject->IsStopWhenAbilityEnd = bStopWhenAbilityEnd;
		AsyncTaskObject->AnimRootMotionTranslationScale = InAnimRootMotionTranslationScale;
	}
	return AsyncTaskObject;
}

void UAsyncTask_PlayMontageAndWaitEvent::Activate()
{
	if (!Ability) return;
	bool IsPlayedMontage = false;
	const auto ActorInfo = Ability->GetCurrentActorInfo();

	if (!ActorInfo) return;
	const auto AnimInstance = ActorInfo->GetAnimInstance();

	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayMontageAndWaitEvent_Async Can't get AnimInstance from ActorInfo"));
		return;
	}
	const auto ASC = GetTargetASC();
	
	if (!ASC) return;
	EventHandle = ASC->AddGameplayEventTagContainerDelegate(EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnGameplayEvent));

	if (ASC->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate, StartSection) > 0.f)
	{
		if (ShouldBroadcastAbilityTaskDelegates() == false) return;

		CancelledHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &ThisClass::OnAbilityCancelled);

		OnBlendingOutStartedDelegate.BindUObject(this, &ThisClass::OnMontageBlendingOutStarted);
		AnimInstance->Montage_SetBlendingOutDelegate(OnBlendingOutStartedDelegate, MontageToPlay);

		OnMontageEndedDelegate.BindUObject(this, &ThisClass::OnMontageEnded);
		AnimInstance->Montage_SetEndDelegate(OnMontageEndedDelegate, MontageToPlay);

		const auto Character = Cast<ACharacter>(GetAvatarActor());
		if (Character &&
			(Character->GetLocalRole() == ROLE_Authority ||
				Character->GetLocalRole() == ROLE_AutonomousProxy ||
				Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted))
		{
			Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
		}
		IsPlayedMontage = true;
	}

	if (!IsPlayedMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayMontageAndWaitEvent_Async Failed to start montage"));

		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnMontageCancelledDelegate.Broadcast(FGameplayTag{}, FGameplayEventData{});
		}
	}

	SetWaitingOnAvatar();
}

void UAsyncTask_PlayMontageAndWaitEvent::ExternalCancel()
{
	if (AbilitySystemComponent)
	{
		OnAbilityCancelled();
		Super::ExternalCancel();
	}
}

void UAsyncTask_PlayMontageAndWaitEvent::OnDestroy(const bool AbilityEnded)
{
	if (Ability)
	{
		Ability->OnGameplayAbilityCancelled.Remove(CancelledHandle);

		if (AbilityEnded && IsStopWhenAbilityEnd)
		{
			if (EndAbilityTask()){}
		}
	}

	if (const auto ASC = GetTargetASC())
	{
		ASC->RemoveGameplayEventTagContainerDelegate(EventTags, EventHandle);
	}
	
	Super::OnDestroy(AbilityEnded);
}

void UAsyncTask_PlayMontageAndWaitEvent::OnMontageBlendingOutStarted(UAnimMontage* Montage, const bool bInterrupted) const
{
	if (Ability && Ability->GetCurrentMontage() == MontageToPlay && MontageToPlay == Montage)
	{
		AbilitySystemComponent->ClearAnimatingAbility(Ability);

		const auto Character = Cast<ACharacter>(GetAvatarActor());
		if (Character &&
			(Character->GetLocalRole() == ROLE_Authority ||
				Character->GetLocalRole() == ROLE_AutonomousProxy ||
				Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted))
		{
			Character->SetAnimRootMotionTranslationScale(1.0f);
		}
	}

	if (bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnMontageInterruptedDelegate.Broadcast(FGameplayTag{}, FGameplayEventData{});
		}
	}
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnMontageBlendOutDelegate.Broadcast(FGameplayTag{}, FGameplayEventData{});
		}
	}
}

void UAsyncTask_PlayMontageAndWaitEvent::OnMontageEnded(UAnimMontage* Montage, const bool bInterrupted)
{
	if (bInterrupted && ShouldBroadcastAbilityTaskDelegates())
	{
		OnMontageCompletedDelegate.Broadcast(FGameplayTag{}, FGameplayEventData{});
	}
	EndTask();
}

void UAsyncTask_PlayMontageAndWaitEvent::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload) const
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		auto TempData = *Payload;
		TempData.EventTag = EventTag;
		OnEventReceivedDelegate.Broadcast(EventTag, TempData);
	}
}

void UAsyncTask_PlayMontageAndWaitEvent::OnAbilityCancelled() const
{
	if (EndAbilityTask() && ShouldBroadcastAbilityTaskDelegates())
	{
		OnMontageCancelledDelegate.Broadcast(FGameplayTag{}, FGameplayEventData{});
	}
}

bool UAsyncTask_PlayMontageAndWaitEvent::EndAbilityTask() const
{
	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (!ActorInfo)
	{
		return false;
	}

	const UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return false;
	}

	if (AbilitySystemComponent && Ability)
	{
		if (AbilitySystemComponent->GetAnimatingAbility() == Ability
			&& AbilitySystemComponent->GetCurrentMontage() == MontageToPlay)
		{
			FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
			if (MontageInstance)
			{
				MontageInstance->OnMontageBlendingOutStarted.Unbind();
				MontageInstance->OnMontageEnded.Unbind();
			}

			AbilitySystemComponent->CurrentMontageStop();
			return true;
		}
	}
	return false;
}

USR_AbilitySystemComponent* UAsyncTask_PlayMontageAndWaitEvent::GetTargetASC() const
{
	return Cast<USR_AbilitySystemComponent>(AbilitySystemComponent);
}
