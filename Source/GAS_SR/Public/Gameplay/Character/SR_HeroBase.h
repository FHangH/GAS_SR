#pragma once

#include "CoreMinimal.h"
#include "SR_CharacterBase.h"
#include "SR_HeroBase.generated.h"

UCLASS()
class GAS_SR_API ASR_HeroBase : public ASR_CharacterBase
{
	GENERATED_BODY()

public:
	ASR_HeroBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
