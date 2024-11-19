#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SR_AbilitySystemComponent.generated.h"

class USR_GameplayAbilityBase;

UCLASS(ClassGroup=(FH), meta=(BlueprintSpawnableComponent))
class GAS_SR_API USR_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	/* Property */
protected:
	TMap<FString, FGameplayAbilitySpec*> Map_AbilitySpec;

public:
	bool IsHasGiveCharacterStartUpAbilities { false };
	
	/* Function */
public:
	USR_AbilitySystemComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, Category="SR|GAS|ASC")
	bool AddNewAbility(TSubclassOf<USR_GameplayAbilityBase> AbilityClass, const int32 Level);
};
