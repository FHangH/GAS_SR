#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Data/SR_Types.h"
#include "Gameplay/GAS/Attributes/SR_AttributeSet.h"
#include "GEEC_Damage.generated.h"

USTRUCT(BlueprintType)
struct FSR_DamageStatic
{
	GENERATED_BODY()

	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack_Physics);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack_Fire);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack_Ice);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage_Physics);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage_Fire);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage_Ice);

	FSR_DamageStatic(): ArmorProperty(nullptr), HealthProperty(nullptr)
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(USR_AttributeSet, Attack, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USR_AttributeSet, Attack_Physics, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USR_AttributeSet, Attack_Fire, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USR_AttributeSet, Attack_Ice, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USR_AttributeSet, Damage_Physics, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USR_AttributeSet, Damage_Fire, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USR_AttributeSet, Damage_Ice, Target, false);
	}
};

static const FSR_DamageStatic& DamageStatics()
{
	static FSR_DamageStatic Out;
	return Out;
}

UCLASS(BlueprintType, Blueprintable)
class GAS_SR_API UGEEC_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(EditAnywhere, Category="A_SR|GEEC")
	bool IsDebug { true };
	
	/* Function */
public:
	UGEEC_Damage();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent, Category = "A_SR|GEEC")
	float CalcPhysicsMitigatedDamage(ESRDamageStatus& OutDamageStatus, float  UnmitigatedDamage, float TargetArmor, float CritPro, float CritMul, float ReducePercent = 0.0f, float PhysicsResistance = 0.0f)const;

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent, Category = "A_SR|GEEC")
	float CalcMagicMitigatedDamage(ESRDamageStatus& OutDamageStatus, float UnmitigatedDamage, float CritPro, float CritMul, float ReducePercent = 0.0f, float Resistance = 0.0f)const;
};
