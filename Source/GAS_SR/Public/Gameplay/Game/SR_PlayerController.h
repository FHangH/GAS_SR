#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SR_PlayerController.generated.h"

UCLASS()
class GAS_SR_API ASR_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void AcknowledgePossession(APawn* P) override;
};
