#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Data/SR_AbilityTypes.h"
#include "GameFramework/Actor.h"
#include "Interface/SR_InteractableObj.h"
#include "SR_SkillActor.generated.h"

UCLASS()
class GAS_SR_API ASR_SkillActor : public AActor, public ISR_InteractableObj
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(BluEprintReadOnly, VisibleAnywhere, Category = "A_SR|SkillActor", meta = (ExposeOnSpawn = true))
	FSR_GameplayEffectContainer Container;

	UPROPERTY(BluEprintReadOnly, VisibleAnywhere, Category = "A_SR|SkillActor", meta = (ExposeOnSpawn = true))
	FGameplayEventData EventData;

	UPROPERTY(BluEprintReadOnly, VisibleAnywhere, Category = "A_SR|SkillActor", meta = (ExposeOnSpawn = true))
	int32 Level;
	
	/* Function */
public:
	ASR_SkillActor();

protected:
	virtual void BeginPlay() override;
};
