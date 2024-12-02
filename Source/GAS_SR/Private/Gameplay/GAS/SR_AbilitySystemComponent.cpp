#include "GAS_SR/Public/Gameplay/GAS/SR_AbilitySystemComponent.h"
#include "Gameplay/Character/SR_CharacterBase.h"
#include "GAS_SR/Public/Gameplay/GAS/GA/SR_GameplayAbilityBase.h"
#include "Utils/FuncLib_Utils.h"

USR_AbilitySystemComponent::USR_AbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USR_AbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USR_AbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool USR_AbilitySystemComponent::AddNewAbility(const TSubclassOf<USR_GameplayAbilityBase> AbilityClass, const int32 Level)
{
	if (!AbilityClass) return false;
	if (AbilityClass && Map_AbilitySpec.Num() > 0)
	{
		for (const auto& AbilitySpec : Map_AbilitySpec)
		{
			if (AbilitySpec.Value->Ability->GetClass() == AbilityClass) return false;
		}
	}

	const auto InLevel = FMath::Clamp(Level, 0, Level);
	const auto AbilitySpec = FGameplayAbilitySpec
	{
		AbilityClass, InLevel, static_cast<int32>(AbilityClass.GetDefaultObject()->AbilityInputID), GetOwnerActor()
	};

	if (IsOwnerActorAuthoritative())
	{
		GiveAbility(AbilitySpec);
		Map_AbilitySpec.Add(AbilityClass.GetDefaultObject()->AbilityName, FindAbilitySpecFromClass(AbilityClass));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySystemComponent::AddNewAbility - Ability not given on client"));
		return false;
	}
	return true;
}

bool USR_AbilitySystemComponent::RemoveAbilityByClass(const TSubclassOf<USR_GameplayAbilityBase> AbilityClass)
{
	if (AbilityClass && IsOwnerActorAuthoritative())
	{
		if (const auto AbilitySpec = FindAbilitySpecFromClass(AbilityClass))
		{
			ClearAbility(AbilitySpec->Handle);
			
			for (const auto& AbilitySpecPair : Map_AbilitySpec)
			{
				if (AbilitySpecPair.Value->Ability->GetClass() == AbilityClass)
				{
					Map_AbilitySpec.Remove(AbilitySpecPair.Key);
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySystemComponent::RemoveAbilityByClass - Ability not removed on client"));
		return false;
	}
	return true;
}

bool USR_AbilitySystemComponent::RemoveAbilityByName(const FString& AbilityName)
{
	if (!AbilityName.IsEmpty() && IsOwnerActorAuthoritative() && IsHasAbility(AbilityName))
	{
		if (const auto AbilitySpec = *Map_AbilitySpec.Find(AbilityName))
		{
			ClearAbility(AbilitySpec->Handle);
			Map_AbilitySpec.Remove(AbilityName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySystemComponent::RemoveAbilityByName - Ability not removed on client"));
		return false;
	}
	return true;
}

int32 USR_AbilitySystemComponent::GetAbilityLevelByClass(const TSubclassOf<USR_GameplayAbilityBase> AbilityClass)
{
	if (const auto AbilitySpec = FindAbilitySpecFromClass(AbilityClass))
	{
		return AbilitySpec->Level <= 0 ? -1 : AbilitySpec->Level;
	}
	return -1;
}

int32 USR_AbilitySystemComponent::GetAbilityLevelByName(const FString& AbilityName)
{
	if (IsHasAbility(AbilityName))
	{
		return (*Map_AbilitySpec.Find(AbilityName))->Level <= 0 ? -1 : (*Map_AbilitySpec.Find(AbilityName))->Level;
	}
	return -1;
}

void USR_AbilitySystemComponent::UpgradeAbilityByClass(const TSubclassOf<USR_GameplayAbilityBase> AbilityClass, const int32 UpLevel)
{
	/*// 教程的写法
	for (auto& Spec : Map_AbilitySpec)
	{
		if (Spec.Value->Ability->GetClass() == AbilityClass)
		{
			Spec.Value->Level += UpLevel;
			MarkAbilitySpecDirty(*Spec.Value);
		}
	}*/

	// 自己的写法
	if (const auto Spec = FindAbilitySpecFromClass(AbilityClass))
	{
		Spec->Level += UpLevel;
		MarkAbilitySpecDirty(*Spec);
	}
}

void USR_AbilitySystemComponent::UpgradeAbilityByName(const FString& AbilityName, const int32 UpLevel)
{
	if (IsHasAbility(AbilityName))
	{
		/*// 教程的写法
		const auto Spec = *Map_AbilitySpec.Find(AbilityName);
		Spec->Level += UpLevel;
		MarkAbilitySpecDirty(*Spec);*/

		// 自己的写法
		for (auto& Spec : GetActivatableAbilities())
		{
			if (Spec.Ability->GetClass()->GetDefaultObject<USR_GameplayAbilityBase>()->AbilityName == AbilityName)
			{
				Spec.Level += UpLevel;
				MarkAbilitySpecDirty(Spec);
			}
		}
	}
}

void USR_AbilitySystemComponent::ReceiveDodge(USR_AbilitySystemComponent* SourceASC, const FSRDodgeParameter& Parameter) const
{
	/*if (IsDebugLog)
	{
		SRLOGEX3(TEXT("%s 闪避了 %s 的攻击, 扩展参数: %s"), *GetAvatarActor()->GetName(), *SourceASC->GetAvatarActor()->GetName(),
			*Parameter.ExtraInfo);
	}*/
	
	OnDodgeDelegate.Broadcast(SourceASC, Parameter);
	const auto Role = Cast<ASR_CharacterBase>(GetAvatarActor());
	if (Role)
	{
		Role->HandleDodge(SourceASC, Parameter);
	}
}

void USR_AbilitySystemComponent::ReceiveDamage(USR_AbilitySystemComponent* SourceASC, const FSRDamageParameter& Parameter) const
{
	/*if (IsDebugLog)
	{
		SRLOGEX5(TEXT("%s 收到 %s 的 [%s] 攻击, 税前伤害 = %f, 税后= %f"), *GetAvatarActor()->GetName(), *SourceASC->GetAvatarActor()->GetName(),
			*UFuncLib_Utils::DamageTypeToString(Parameter.Type), Parameter.UnmitigatedDamage, Parameter.MitigatedDamage);
	}*/
	
	OnDamageDelegate.Broadcast(SourceASC, Parameter);
	const auto Role = Cast<ASR_CharacterBase>(GetAvatarActor());
	if (Role)
	{
		Role->HandleDamage(SourceASC, Parameter);
		if (!Role->IsAlive())
		{
			Role->HandleDeath(SourceASC, Parameter);
		}
	}
}

void USR_AbilitySystemComponent::ReceiveBlock(USR_AbilitySystemComponent* SourceASC, const FSRBlockParameter& Parameter) const
{
	/*if (IsDebugLog)
	{
		SRLOGEX4(TEXT("%s 格挡了 %s 攻击的 %f 点伤害(格挡前伤害= %f)"), *GetAvatarActor()->GetName(), *SourceASC->GetAvatarActor()->GetName(), Parameter.BlockValue, Parameter.UnmitigatedDamage);
	}*/
	
	OnBlockDelegate.Broadcast(SourceASC, Parameter);
	const auto Role = Cast<ASR_CharacterBase>(GetAvatarActor());
	if (Role)
	{
		Role->HandleBlock(SourceASC, Parameter);
	}
}

void USR_AbilitySystemComponent::Server_AddNewAbility_Implementation(const TSubclassOf<USR_GameplayAbilityBase> AbilityClass, const int32 Level)
{
	AddNewAbility(AbilityClass, Level);
}

void USR_AbilitySystemComponent::Server_RemoveAbilityByClass_Implementation(const TSubclassOf<USR_GameplayAbilityBase> AbilityClass)
{
	RemoveAbilityByClass(AbilityClass);
}

void USR_AbilitySystemComponent::Server_RemoveAbilityByName_Implementation(const FString& AbilityName)
{
	RemoveAbilityByName(AbilityName);
}

void USR_AbilitySystemComponent::Server_UpgradeAbilityByClass_Implementation(const TSubclassOf<USR_GameplayAbilityBase> AbilityClass, const int32 UpLevel)
{
	UpgradeAbilityByClass(AbilityClass, UpLevel);
}

void USR_AbilitySystemComponent::Server_UpgradeAbilityByName_Implementation(const FString& AbilityName, const int32 UpLevel)
{
	UpgradeAbilityByName(AbilityName, UpLevel);
}