#include "GAS_SR/Public/Gameplay/Game/SR_PlayerController.h"
#include "AbilitySystemComponent.h"
#include "GAS_SR/Public/Gameplay/Character/SR_CharacterBase.h"

void ASR_PlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	if (const auto SRCharacter = Cast<ASR_CharacterBase>(P))
	{
		SRCharacter->GetAbilitySystemComponent()->InitAbilityActorInfo(SRCharacter, SRCharacter);
	}
}
