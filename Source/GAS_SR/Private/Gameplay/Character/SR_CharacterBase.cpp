#include "GAS_SR/Public/Gameplay/Character/SR_CharacterBase.h"
#include "GAS_SR/Public/Gameplay/GAS/SR_AbilitySystemComponent.h"
#include "GAS_SR/Public/Gameplay/GAS/Attributes/SR_AttributeSet.h"

ASR_CharacterBase::ASR_CharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<USR_AbilitySystemComponent>(TEXT("SR_AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<USR_AttributeSet>(TEXT("SR_AttributeSet"));
}

void ASR_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASR_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASR_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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
}
