#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "SR_AttributeSet.generated.h"

/**
 * 这定义了一组用于访问和初始化属性的 helper 函数，以避免必须手动编写这些函数。
 * 它将为属性 Health 创建以下函数
 *
 *	static FGameplayAttribute UMyHealthSet::GetHealthAttribute();
 *	FORCEINLINE float UMyHealthSet::GetHealth() const;
 *	FORCEINLINE void UMyHealthSet::SetHealth(float NewVal);
 *	FORCEINLINE void UMyHealthSet::InitHealth(float NewVal);
 *
 * 要在游戏中使用它，您可以定义如下内容，然后根据需要添加特定于游戏的函数：
 * 
 *	#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 *	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 *	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 *	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 *	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
 * 
 *	ATTRIBUTE_ACCESSORS(UMyHealthSet, Health)
 *
 *	======================================================================================================================
 *
 *	这是一个帮助程序宏，可在 RepNotify 函数中使用，以处理客户端将预测性修改的属性。
 *	
 *	void UMyHealthSet::OnRep_Health(const FGameplayAttributeData& OldValue)
 *	{
 *		GAMEPLAYATTRIBUTE_REPNOTIFY(UMyHealthSet, Health, OldValue);
 *	}
 *
 *	======================================================================================================================
 *
 *	有助于从电子表格 （UCurveTable） 初始化属性集默认值的辅助结构体。
 * 项目可以根据需要自由初始化其属性集。这只是想要的示例，即
 * 在某些情况下很有用。
 *	
 * 基本思路是拥有此表单的电子表格：
 *	
 *									1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17	18	19	20
 *
 *	Default.Health.MaxHealth		100	200	300	400	500	600	700	800	900	999	999	999	999	999	999	999	999	999	999	999
 *	Default.Health.HealthRegenRate	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1
 *	Default.Health.AttackRating		10	10	10	10	10	10	10	10	10	10	10	10	10	10	10	10	10	10	10	10
 *	Default.Move.MaxMoveSpeed		500	500	500	500	500	500	500	500	500	500	500	500	500	500	500	500	500	500	500	500
 *	Hero1.Health.MaxHealth			100	100	100	100	100	100	100	100	100	100	100	100	100	100	100	100	100	100	100	100
 *	Hero1.Health.HealthRegenRate	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1 	1	1	1	1
 *	Hero1.Health.AttackRating		10	10	10	10	10	10	10	10	10	10	10	10	10	10	10	10	10	10	10	10
 *	Hero1.Move.MaxMoveSpeed			500	500	500	500	500	500	500	500	500	500	500	500	500	500	500	500	500	500	500	500
 *	
 *	
 *	行在表单中的位置: [GroupName].[AttributeSetName].[Attribute]
 *	GroupName			- 任意名称来标识 "group"
 *	AttributeSetName	- 属性属于什么 UAttributeSet. (请注意，这是对 UClass 名称的简单部分匹配. "Health" 匹配 "UMyGameHealthSet").
 *	Attribute			- 实际特性属性的名称 (匹配全名).
 *		
 *	列表示 “Level”。
 *	
 *	FAttributeSetInitter::PreloadAttributeSetData(UCurveTable*)
 *	这会将 CurveTable 转换为更高效的格式，以便在运行时读入。应从 UAbilitySystemGlobals 例如.
 *
 *	FAttributeSetInitter::InitAttributeSetDefaults(UAbilitySystemComponent* AbilitySystemComponent, FName GroupName, int32 Level) const;
 *	这将使用指定的 GroupName 和 Level 初始化给定的 AbilitySystemComponent 的属性集。游戏代码将调用
 * 在生成新 Actor 或升级 Actor 等时执行此操作。
 *	
 *	游戏代码使用示例：
 *	
 *	IGameplayAbilitiesModule::Get().GetAbilitySystemGlobals()->GetAttributeSetInitter()->InitAttributeSetDefaults(MyCharacter->AbilitySystemComponent, "Hero1", MyLevel);
 *	
 *	笔记：
 * -这允许系统设计人员为属性指定任意值。他们可以基于他们想要的任何公式。
 * -具有非常大的级别上限的项目可能希望采用更简单的 "Attributes gained per level" 方法.
 *	-在此方法中初始化的任何内容都不应由游戏效果直接修改。例如, 如果 MaxMoveSpeed scales with level,其他任何 
 *		修改 MaxMoveSpeed 应该使用非即时 GameplayEffect 执行此操作.
 *	-"Default" 是当前硬编码的回退 GroupName。如果 InitAttributeSetDefaults 在没有有效 GroupName 的情况下调用，我们将回退到默认的.
 *
 */

class ASR_CharacterBase;

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

#define DAMAGE_HANDLE(Attribute,C,Name,TempDamage,TempNewHp)\
{\
	if(Attribute == GetDamage_##Name##Attribute()) \
	{	\
		TempDamage = GetDamage_##Name(); \
		SetDamage_##Name(0);	\
		if(TempDamage>0)	\
		{	\
			ASR_CharacterBase* character = Cast<ASR_CharacterBase>(C);	\
			if (character&&character->IsAlive())\
			{\
				TempNewHp = GetHealth() - TempDamage;\
			    SetHealth(TempNewHp);\
			}\
		}	\
	} \
}

UCLASS()
class GAS_SR_API USR_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="A_SR|Attribute")
	FGameplayAttributeData Health { 100.f };
	ATTRIBUTE_ACCESSORS(USR_AttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="A_SR|Attribute")
	FGameplayAttributeData MaxHealth { 100.f };
	ATTRIBUTE_ACCESSORS(USR_AttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mana, Category="A_SR|Attribute")
	FGameplayAttributeData Mana { 100.f };
	ATTRIBUTE_ACCESSORS(USR_AttributeSet, Mana)
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxMana, Category="A_SR|Attribute")
	FGameplayAttributeData MaxMana { 100.f };
	ATTRIBUTE_ACCESSORS(USR_AttributeSet, MaxMana)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Attack, Category="A_SR|Attribute")
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(USR_AttributeSet, Attack)
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Armor, Category="A_SR|Attribute")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(USR_AttributeSet, Armor)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Strength, Category="A_SR|Attribute")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(USR_AttributeSet, Strength)

	//临时攻击数据, 通过技能初始化, 用于结算最终伤害
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Attack_Physics, Category="A_SR|Attribute")
	FGameplayAttributeData Attack_Physics;
	ATTRIBUTE_ACCESSORS(USR_AttributeSet, Attack_Physics)
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Attack_Fire, Category="A_SR|Attribute")
	FGameplayAttributeData Attack_Fire;
	ATTRIBUTE_ACCESSORS(USR_AttributeSet, Attack_Fire)
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Attack_Ice, Category="A_SR|Attribute")
	FGameplayAttributeData Attack_Ice;
	ATTRIBUTE_ACCESSORS(USR_AttributeSet, Attack_Ice)

	//临时受伤数据
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Damage_Physics, Category="A_SR|Attribute")
	FGameplayAttributeData Damage_Physics;
	ATTRIBUTE_ACCESSORS(USR_AttributeSet, Damage_Physics)
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Damage_Fire, Category="A_SR|Attribute")
	FGameplayAttributeData Damage_Fire;
	ATTRIBUTE_ACCESSORS(USR_AttributeSet, Damage_Fire)
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Damage_Ice, Category="A_SR|Attribute")
	FGameplayAttributeData Damage_Ice;
	ATTRIBUTE_ACCESSORS(USR_AttributeSet, Damage_Ice)

	/* Function */
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	void AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty) const;
	void HandleSecondaryAttributeChanged(const FGameplayAttribute& Attribute);

	void HandleDamage(const FGameplayAttribute& Attribute, ASR_CharacterBase* TargetCharacter);
	static bool IsAnyDamage(const FGameplayAttribute& Attribute);
	
	// Attribute OnRep
protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_Attack(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_Armor(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_Strength(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Attack_Physics(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_Attack_Fire(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_Attack_Ice(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_Damage_Physics(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_Damage_Fire(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_Damage_Ice(const FGameplayAttributeData& OldValue);
};
