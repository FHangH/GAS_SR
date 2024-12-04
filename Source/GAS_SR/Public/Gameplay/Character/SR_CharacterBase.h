#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Data/SR_Types.h"
#include "GameFramework/Character.h"
#include "Gameplay/GAS/Attributes/SR_AttributeSet.h"
#include "SR_CharacterBase.generated.h"

class UGameplayEffect;
class USR_GameplayAbilityBase;
class USR_AttributeSet;
class USR_AbilitySystemComponent;

UCLASS()
class GAS_SR_API ASR_CharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category="A_SR|Character|GAS")
	USR_AbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category="A_SR|Character|GAS")
	USR_AttributeSet* AttributeSet;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="A_SR|Character|GAS")
	TArray<TSubclassOf<USR_GameplayAbilityBase>> CharacterAbilities;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="A_SR|Character|GAS")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffects;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="A_SR|Character|GAS")
	TArray<TSubclassOf<UGameplayEffect>> StartUpEffects;
	
	/* Function */
public:
	explicit ASR_CharacterBase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void InitAttributes();
	virtual void AddCharacterStartUpAbilities();
	virtual void AddStatUpEffects();

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "A_SR|Character|Handle")
	void HandleDodge(USR_AbilitySystemComponent* SourceASC, const FSRDodgeParameter& Parameter);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "A_SR|Character|Handle")
	void HandleDamage(USR_AbilitySystemComponent* SourceASC, const FSRDamageParameter& Parameter);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "A_SR|Character|Handle")
	void HandleBlock(USR_AbilitySystemComponent* SourceASC, const FSRBlockParameter& Parameter);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "A_SR|Character|Handle")
	void HandleDeath(USR_AbilitySystemComponent* SourceASC, const FSRDamageParameter& Parameter);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Status")
	FORCEINLINE bool IsAlive() const { return GetHealth() > 0.f; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attribute")
	FORCEINLINE float GetHealth() const { return AttributeSet ? AttributeSet->GetHealth() : 0.f; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attribute")
	FORCEINLINE float GetMaxHealth() const { return AttributeSet ? AttributeSet->GetMaxHealth() : 0.f; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attribute")
	FORCEINLINE float GetMana() const { return AttributeSet ? AttributeSet->GetMana() : 0.f; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attribute")
	FORCEINLINE float GetMaxMana() const { return AttributeSet ? AttributeSet->GetMaxMana() : 0.f; }
};
