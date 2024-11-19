#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
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
	ASR_CharacterBase();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void InitAttributes();
	virtual void AddCharacterStartUpAbilities();
	virtual void AddStatUpEffects();
};
