#include "GAS_SR/Public/Gameplay/GAS/Attributes/SR_AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Gameplay/Character/SR_CharacterBase.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "Math/UnrealMathUtility.h"

void USR_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USR_AttributeSet, Health);
	DOREPLIFETIME(USR_AttributeSet, MaxHealth);
	DOREPLIFETIME(USR_AttributeSet, Mana);
	DOREPLIFETIME(USR_AttributeSet, MaxMana);
	DOREPLIFETIME(USR_AttributeSet, Attack);
	DOREPLIFETIME(USR_AttributeSet, Armor);
	DOREPLIFETIME(USR_AttributeSet, Strength);
	DOREPLIFETIME(USR_AttributeSet, Attack_Physics);
	DOREPLIFETIME(USR_AttributeSet, Attack_Fire);
	DOREPLIFETIME(USR_AttributeSet, Attack_Ice);
	DOREPLIFETIME(USR_AttributeSet, Damage_Physics);
	DOREPLIFETIME(USR_AttributeSet, Damage_Fire);
	DOREPLIFETIME(USR_AttributeSet, Damage_Ice);
}

void USR_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
	}
}

void USR_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	const UAbilitySystemComponent* SourceASC = Context.GetOriginalInstigatorAbilitySystemComponent();
	FGameplayTagContainer SpecAssetTags;
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

	ASR_CharacterBase* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		AActor* TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetCharacter = Cast<ASR_CharacterBase>(TargetActor);
	}

	if (SourceASC && SourceASC->AbilityActorInfo.IsValid() && SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		HandleSecondaryAttributeChanged(Data.EvaluatedData.Attribute);
		
		if (Data.EvaluatedData.Attribute == GetHealthAttribute())
		{
			SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
		}
		else if (Data.EvaluatedData.Attribute == GetManaAttribute())
		{
			SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
		}

		if (IsAnyDamage(Data.EvaluatedData.Attribute))
		{
			HandleDamage(Data.EvaluatedData.Attribute, TargetCharacter);
		}
	}
}

void USR_AttributeSet::AdjustAttributeForMaxChange(
	const FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute,
	const float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty) const
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// 更改当前值以保持 Cur / Max 的百分比
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		const float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void USR_AttributeSet::HandleSecondaryAttributeChanged(const FGameplayAttribute& Attribute)
{
	if (Attribute == GetStrengthAttribute())
	{
		SetAttack(FMath::Clamp(GetStrength() * 1.0f, 0.0f ,99999.f));
	}
}

void USR_AttributeSet::HandleDamage(const FGameplayAttribute& Attribute, ASR_CharacterBase* TargetCharacter)
{
	float Damage;
	float NewHp;
	DAMAGE_HANDLE(Attribute, TargetCharacter, Physics, Damage, NewHp);
	DAMAGE_HANDLE(Attribute, TargetCharacter, Fire, Damage, NewHp);
	DAMAGE_HANDLE(Attribute, TargetCharacter, Ice, Damage, NewHp);
}

bool USR_AttributeSet::IsAnyDamage(const FGameplayAttribute& Attribute)
{
	return Attribute == GetDamage_FireAttribute() ||
			Attribute == GetDamage_IceAttribute() ||
				Attribute == GetDamage_PhysicsAttribute();
}

void USR_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USR_AttributeSet, Health, OldValue);
}

void USR_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USR_AttributeSet, MaxHealth, OldValue);
}

void USR_AttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USR_AttributeSet, Mana, OldValue);
}

void USR_AttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USR_AttributeSet, MaxMana, OldValue);
}

void USR_AttributeSet::OnRep_Attack(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USR_AttributeSet, Attack, OldValue);
}

void USR_AttributeSet::OnRep_Armor(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USR_AttributeSet, Armor, OldValue);
}

void USR_AttributeSet::OnRep_Strength(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USR_AttributeSet, Strength, OldValue);
}

void USR_AttributeSet::OnRep_Attack_Physics(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USR_AttributeSet, Attack_Physics, OldValue);
}

void USR_AttributeSet::OnRep_Attack_Fire(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USR_AttributeSet, Attack_Fire, OldValue);
}

void USR_AttributeSet::OnRep_Attack_Ice(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USR_AttributeSet, Attack_Ice, OldValue);
}

void USR_AttributeSet::OnRep_Damage_Physics(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USR_AttributeSet, Damage_Physics, OldValue);
}

void USR_AttributeSet::OnRep_Damage_Fire(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USR_AttributeSet, Damage_Fire, OldValue);
}

void USR_AttributeSet::OnRep_Damage_Ice(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USR_AttributeSet, Damage_Ice, OldValue);
}