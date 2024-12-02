#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Data/SR_Types.h"
#include "SR_AbilitySystemComponent.generated.h"

class USR_GameplayAbilityBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRecieveEventDodgeSignature, USR_AbilitySystemComponent*, SourceASC, const FSRDodgeParameter&, Parameter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReceiveEventBlockSignature, USR_AbilitySystemComponent*, SourceASC, const FSRBlockParameter&, Parameter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReceiveEventDamageSignature, USR_AbilitySystemComponent*, SourceASC, const FSRDamageParameter&, Parameter);

UCLASS(ClassGroup=(FH), meta=(BlueprintSpawnableComponent))
class GAS_SR_API USR_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	/* Property */
protected:
	TMap<FString, FGameplayAbilitySpec*> Map_AbilitySpec;

public:
	bool IsHasGiveCharacterStartUpAbilities { false };
	bool IsHasApplyStartUpEffects { false };

	UPROPERTY(EditAnywhere, Category = "SR|GAS|ASC")
	bool IsDebugLog { true };

	UPROPERTY(BlueprintAssignable)
	FOnRecieveEventDodgeSignature OnDodgeDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnReceiveEventDamageSignature OnDamageDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnReceiveEventBlockSignature OnBlockDelegate;
	
	/* Function */
public:
	USR_AbilitySystemComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, Category="SR|GAS|ASC")
	bool AddNewAbility(TSubclassOf<USR_GameplayAbilityBase> AbilityClass, const int32 Level);
	
	UFUNCTION(BlueprintCallable, Category="SR|GAS|ASC")
	bool RemoveAbilityByClass(TSubclassOf<USR_GameplayAbilityBase> AbilityClass);
	
	UFUNCTION(BlueprintCallable, Category="SR|GAS|ASC")
	bool RemoveAbilityByName(const FString& AbilityName);

	UFUNCTION(BlueprintPure, Category="SR|GAS|ASC")
	FORCEINLINE bool IsHasAbility(const FString& AbilityName) const { return Map_AbilitySpec.Contains(AbilityName); }

	UFUNCTION(BlueprintPure, Category="SR|GAS|ASC")
	int32 GetAbilityLevelByClass(TSubclassOf<USR_GameplayAbilityBase> AbilityClass);
	
	UFUNCTION(BlueprintPure, Category="SR|GAS|ASC")
	int32 GetAbilityLevelByName(const FString& AbilityName);

	UFUNCTION(BlueprintCallable, Category="SR|GAS|ASC")
	void UpgradeAbilityByClass(TSubclassOf<USR_GameplayAbilityBase> AbilityClass, const int32 UpLevel = 1);

	UFUNCTION(BlueprintCallable, Category="SR|GAS|ASC")
	void UpgradeAbilityByName(const FString& AbilityName, const int32 UpLevel = 1);

	void ReceiveDodge(USR_AbilitySystemComponent* SourceASC, const FSRDodgeParameter& Parameter) const;
	void ReceiveDamage(USR_AbilitySystemComponent* SourceASC, const FSRDamageParameter& Parameter) const;
	void ReceiveBlock(USR_AbilitySystemComponent* SourceASC, const FSRBlockParameter& Parameter) const;

	// RPC
	UFUNCTION(BlueprintCallable, Server, Reliable, Category="SR|GAS|ASC")
	void Server_AddNewAbility(TSubclassOf<USR_GameplayAbilityBase> AbilityClass, const int32 Level);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category="SR|GAS|ASC")
	void Server_RemoveAbilityByClass(TSubclassOf<USR_GameplayAbilityBase> AbilityClass);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category="SR|GAS|ASC")
	void Server_RemoveAbilityByName(const FString& AbilityName);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category="SR|GAS|ASC")
	void Server_UpgradeAbilityByClass(TSubclassOf<USR_GameplayAbilityBase> AbilityClass, const int32 UpLevel = 1);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category="SR|GAS|ASC")
	void Server_UpgradeAbilityByName(const FString& AbilityName, const int32 UpLevel = 1);
};
