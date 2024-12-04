#include "GAS_SR/Public/Gameplay/Character/SR_CharacterBase.h"
#include "GAS_SR/Public/Gameplay/GAS/SR_AbilitySystemComponent.h"
#include "GAS_SR/Public/Gameplay/GAS/Attributes/SR_AttributeSet.h"
#include "Instance/SR_SkillActor.h"

ASR_CharacterBase::ASR_CharacterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<USR_AbilitySystemComponent>(TEXT("SR_AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<USR_AttributeSet>(TEXT("SR_AttributeSet"));
}

void ASR_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASR_CharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitAttributes();
		AddCharacterStartUpAbilities();
		AddStatUpEffects();
	}
}

UAbilitySystemComponent* ASR_CharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ASR_CharacterBase::InitAttributes()
{
	if (!AbilitySystemComponent || !DefaultAttributeEffects)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffects,1, EffectContextHandle);
	if (SpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), AbilitySystemComponent);
	}
}

void ASR_CharacterBase::AddCharacterStartUpAbilities()
{
	if (HasAuthority() && !AbilitySystemComponent->IsHasGiveCharacterStartUpAbilities)
	{
		for (const auto& GameplayAbility : CharacterAbilities)
		{
			AbilitySystemComponent->AddNewAbility(GameplayAbility, 1);
		}
		AbilitySystemComponent->IsHasGiveCharacterStartUpAbilities = true;
	}
}

void ASR_CharacterBase::AddStatUpEffects()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent || AbilitySystemComponent->IsHasApplyStartUpEffects)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	for (const auto GE : StartUpEffects)
	{
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GE, 1, EffectContextHandle);
		if (SpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), AbilitySystemComponent);
		}
	}
	AbilitySystemComponent->IsHasApplyStartUpEffects = true;
}

bool ASR_CharacterBase::TrySpawnSkillActor_Implementation(
	const TSubclassOf<ASR_SkillActor> SkillClass, const FSR_GameplayEffectContainer& Container,
	const FGameplayEventData& EventData, const int32 Level, const FTransform& Transform, APawn* OwnerPawn)
{
	if (SkillClass)
	{
		Server_SpawnSkillActor(SkillClass, Container, EventData, Level, Transform, OwnerPawn ? OwnerPawn : this);
		return true;
	}
	return false;
}

ESRInteractableObjType ASR_CharacterBase::GetInteractType_Implementation() const
{
	return ESRInteractableObjType::Role;
}

ESRInteractableObjCamp ASR_CharacterBase::GetInteractCamp_Implementation() const
{
	return ESRInteractableObjCamp::Monster;
}

void ASR_CharacterBase::HandleDodge_Implementation(USR_AbilitySystemComponent* SourceASC, const FSRDodgeParameter& Parameter)
{
}

void ASR_CharacterBase::HandleDamage_Implementation(USR_AbilitySystemComponent* SourceASC, const FSRDamageParameter& Parameter)
{
}

void ASR_CharacterBase::HandleBlock_Implementation(USR_AbilitySystemComponent* SourceASC, const FSRBlockParameter& Parameter)
{
}

void ASR_CharacterBase::HandleDeath_Implementation(USR_AbilitySystemComponent* SourceASC, const FSRDamageParameter& Parameter)
{
}

void ASR_CharacterBase::Server_SpawnSkillActor_Implementation(
	const TSubclassOf<ASR_SkillActor> SkillClass, const FSR_GameplayEffectContainer& Container,
	const FGameplayEventData& EventData, const int32 Level, const FTransform& Transform, APawn* OwnerPawn)
{
	FActorSpawnParameters SpawnParameter;
	SpawnParameter.Instigator = OwnerPawn;

	const auto Actor = GetWorld()->SpawnActor<ASR_SkillActor>(SkillClass, Transform, SpawnParameter);
	if (Actor)
	{
		Actor->Container = Container;
		Actor->EventData = EventData;
		Actor->Level = Level;
		MultiCast_SpawnSkillActor(SkillClass, Transform, OwnerPawn);
	}
}

void ASR_CharacterBase::MultiCast_SpawnSkillActor_Implementation(
	const TSubclassOf<ASR_SkillActor> SkillClass, const FTransform& Transform, APawn* OwnerPawn)
{
	if (!HasAuthority())
	{
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.Instigator = OwnerPawn;
		GetWorld()->SpawnActor<ASR_SkillActor>(SkillClass, Transform, SpawnParameter);
	}
}