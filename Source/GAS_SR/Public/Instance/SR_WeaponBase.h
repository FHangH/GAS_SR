#pragma once

#include "CoreMinimal.h"
#include "SR_EquipmentBase.h"
#include "GameplayTagContainer.h"
#include "SR_WeaponBase.generated.h"

class UCapsuleComponent;

UCLASS()
class GAS_SR_API ASR_WeaponBase : public ASR_EquipmentBase
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="A_SR|Weapon")
	UCapsuleComponent* TriggerBoxComponent;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="A_SR|Weapon")
	FGameplayTag CurrentEventTag;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="A_SR|Weapon")
	bool IsAttacking = false;
	
	/* Function */
public:
	ASR_WeaponBase();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="A_SR|Weapon", meta=(AutoCreateRefTerm="Tag"))
	void BeginWeaponAttack(const FGameplayTag& Tag);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="A_SR|Weapon")
	void EndWeaponAttack();
};
