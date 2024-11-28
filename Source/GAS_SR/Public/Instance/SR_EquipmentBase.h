#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SR_EquipmentBase.generated.h"

class USphereComponent;
class USkeletalMeshComponent;

UCLASS()
class GAS_SR_API ASR_EquipmentBase : public AActor
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="A_SR|Equipment")
	USkeletalMeshComponent* EquipMeshComponent;
	
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="A_SR|Equipment")
	USphereComponent* RootSphereComponent;
	
	/* Function */
public:
	ASR_EquipmentBase();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="A_SR|Equipment")
	void ResetEquipMesh();
};
