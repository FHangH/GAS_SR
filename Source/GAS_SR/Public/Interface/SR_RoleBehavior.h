#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Data/SR_AbilityTypes.h"
#include "UObject/Interface.h"
#include "SR_RoleBehavior.generated.h"

class ASR_SkillActor;

UINTERFACE()
class USR_RoleBehavior : public UInterface
{
	GENERATED_BODY()
};

class GAS_SR_API ISR_RoleBehavior
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "A_SR|RoleBehavior|Combat", meta = (AutoCreateRefTerm = "Transform"))
	bool TrySpawnSkillActor(TSubclassOf<ASR_SkillActor> SkillClass, const FSR_GameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 Level, const FTransform& Transform, APawn* OwnerPawn);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "A_SR|RoleBehavior|Combat", meta = (AutoCreateRefTerm = "Transform"))
	void SkillBegin();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "A_SR|RoleBehavior|Combat", meta = (AutoCreateRefTerm = "Transform"))
	void SkillEnd();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "A_SR|RoleBehavior|Combat")
	void FaceToPoint(FVector Point);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "A_SR|RoleBehavior|Combat")
	void FaceToTarget(AActor* Target);
};
