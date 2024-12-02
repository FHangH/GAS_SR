#include "Gameplay/GAS/GEEC/GEEC_Damage.h"
#include "Gameplay/GAS/SR_AbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"

UGEEC_Damage::UGEEC_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().AttackDef);
	RelevantAttributesToCapture.Add(DamageStatics().Attack_PhysicsDef);
	RelevantAttributesToCapture.Add(DamageStatics().Attack_FireDef);
	RelevantAttributesToCapture.Add(DamageStatics().Attack_IceDef);
	RelevantAttributesToCapture.Add(DamageStatics().Damage_PhysicsDef);
	RelevantAttributesToCapture.Add(DamageStatics().Damage_FireDef);
	RelevantAttributesToCapture.Add(DamageStatics().Damage_IceDef);
}

void UGEEC_Damage::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const auto TargetAbilitySystemComponent = Cast<USR_AbilitySystemComponent>(ExecutionParams.GetTargetAbilitySystemComponent());
	const auto SourceAbilitySystemComponent = Cast<USR_AbilitySystemComponent>(ExecutionParams.GetSourceAbilitySystemComponent());
	if (!TargetAbilitySystemComponent || !SourceAbilitySystemComponent) return;

	const auto SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	const auto TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;
	if (!SourceActor || !TargetActor) return;
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	FHitResult Hit;
	if (Spec.GetContext().GetHitResult())
	{
		Hit = *Spec.GetContext().GetHitResult();
	}

	float PhysicsAttack = 0.f;
	float FireAttack = 0.f;
	float IceAttack = 0.f;

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().Attack_PhysicsDef, EvaluationParameters, PhysicsAttack);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().Attack_FireDef, EvaluationParameters, FireAttack);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().Attack_IceDef, EvaluationParameters, IceAttack);

	ESRDamageStatus DamageStatus = ESRDamageStatus::Normal;

	float FireDamage = CalcMagicMitigatedDamage(DamageStatus, FireAttack, 0.2, 2, 0, 0);
	if (FireDamage > 0)
	{
		TargetAbilitySystemComponent->ReceiveDamage(SourceAbilitySystemComponent, FSRDamageParameter{Hit, FireAttack, FireDamage, ESRDamageType::Fire, DamageStatus});
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().Damage_FireProperty, EGameplayModOp::Additive, FireDamage));
	}
	
	float IceDamage = CalcMagicMitigatedDamage(DamageStatus, IceAttack, 0.2, 2, 0, 0);
	if (IceDamage > 0)
	{
		TargetAbilitySystemComponent->ReceiveDamage(SourceAbilitySystemComponent, FSRDamageParameter{Hit, IceAttack, IceDamage, ESRDamageType::Ice, DamageStatus});
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().Damage_IceProperty, EGameplayModOp::Additive, IceDamage));
	}
	
	float PhysicsDamage = CalcPhysicsMitigatedDamage(DamageStatus, PhysicsAttack, 0, 0.2, 1.5, 0, 0);
	if (PhysicsDamage > 0)
	{
		TargetAbilitySystemComponent->ReceiveDamage(SourceAbilitySystemComponent, FSRDamageParameter{Hit, PhysicsAttack, PhysicsDamage, ESRDamageType::Physics, DamageStatus});
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().Damage_PhysicsProperty, EGameplayModOp::Additive, PhysicsDamage));
	}
}

float UGEEC_Damage::CalcPhysicsMitigatedDamage_Implementation(
	ESRDamageStatus& OutDamageStatus, const float UnmitigatedDamage, const float TargetArmor, const float CritPro, const float CritMul, const float ReducePercent, const float PhysicsResistance) const
{
	const bool bIsCrit = UKismetMathLibrary::RandomBoolWithWeight(CritPro);
	const float ArmorPerc = 1 - (TargetArmor / (TargetArmor + 500));
	const float GloblePerc = FMath::Clamp((1 - ReducePercent), 0.01f, 100.f);
	const float ResistancePerc = 1 - (PhysicsResistance / (PhysicsResistance + 50));
	const float CritValue = FMath::Clamp((bIsCrit ? CritMul : 1.0f), 1.0f, 999.0f);
	OutDamageStatus = bIsCrit ? ESRDamageStatus::Crit : ESRDamageStatus::Normal;
	const float OutDamage = UnmitigatedDamage * ArmorPerc * GloblePerc * ResistancePerc * CritValue;
	return OutDamage;
}

float UGEEC_Damage::CalcMagicMitigatedDamage_Implementation(
	ESRDamageStatus& OutDamageStatus, const float UnmitigatedDamage, const float CritPro, const float CritMul, const float ReducePercent, const float Resistance) const
{
	const bool bIsCrit = UKismetMathLibrary::RandomBoolWithWeight(CritPro);
	const float GloblePerc = FMath::Clamp((1 - ReducePercent), 0.01f, 100.f);
	const float ResistancePerc = 1 - (Resistance / (Resistance + 50));
	const float CritValue = FMath::Clamp((bIsCrit ? CritMul : 1.0f), 1.0f, 999.0f);
	OutDamageStatus = bIsCrit ? ESRDamageStatus::Crit : ESRDamageStatus::Normal;
	const float OutDamage = UnmitigatedDamage * GloblePerc * ResistancePerc * CritValue;
	return OutDamage;
}
