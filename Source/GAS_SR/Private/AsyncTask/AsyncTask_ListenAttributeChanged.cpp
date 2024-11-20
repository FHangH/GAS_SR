#include "AsyncTask/AsyncTask_ListenAttributeChanged.h"
#include "Gameplay/GAS/Attributes/SR_AttributeSet.h"

UAsyncTask_ListenAttributeChanged* UAsyncTask_ListenAttributeChanged::ListenAttributeChanged(UObject* WorldContext, UAbilitySystemComponent* AbilitySystemComponent, const FGameplayAttribute Attribute)
{
	AsyncTask = NewObject<UAsyncTask_ListenAttributeChanged>();
	if (AsyncTask)
	{
		AsyncTask->WorldContextObject = WorldContext;
		AsyncTask->ASComponent = AbilitySystemComponent;
		AsyncTask->AttributeToListen = Attribute;
		return AsyncTask;
	}
	return nullptr;
}

void UAsyncTask_ListenAttributeChanged::Activate()
{
	AddToRoot();
	RegisterWithGameInstance(WorldContextObject);

	if (!ASComponent || !AttributeToListen.IsValid())
	{
		EndTask();
		return;
	}
	ASComponent->GetGameplayAttributeValueChangeDelegate(AttributeToListen).AddUObject(AsyncTask, &UAsyncTask_ListenAttributeChanged::OnAttributeValueChanged);
}

void UAsyncTask_ListenAttributeChanged::OnAttributeValueChanged(const FOnAttributeChangeData& AttributeChangeData) const
{
	OnAttributeChangedDelegate.Broadcast(AttributeChangeData.Attribute, AttributeChangeData.NewValue, AttributeChangeData.OldValue);
}

void UAsyncTask_ListenAttributeChanged::EndTask()
{
	if (ASComponent && ASComponent->GetGameplayAttributeValueChangeDelegate(AttributeToListen).IsBoundToObject(AsyncTask))
	{
		ASComponent->GetGameplayAttributeValueChangeDelegate(AttributeToListen).RemoveAll(AsyncTask);
	}
	SetReadyToDestroy();
	RemoveFromRoot();
	MarkPendingKill();
}
