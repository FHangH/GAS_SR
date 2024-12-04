#pragma once

#include "CoreMinimal.h"
#include "Data/SR_Types.h"
#include "UObject/Interface.h"
#include "SR_InteractableObj.generated.h"

UINTERFACE()
class USR_InteractableObj : public UInterface
{
	GENERATED_BODY()
};

class GAS_SR_API ISR_InteractableObj
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Keywords = "GetSize"), Category = "A_SR|InteractableObj")
	float GetBodySize() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Keywords = "GetType"), Category = "A_SR|InteractableObj")
	ESRInteractableObjType GetInteractType() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Keywords = "GetCamp"), Category = "A_SR|InteractableObj")
	ESRInteractableObjCamp GetInteractCamp() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "A_SR|InteractableObj")
	void InteractBegin(AController* SourceController, AActor* SourceActor, ESRInteractType Type);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "A_SR|InteractableObj")
	void InteractEnd(AController* SourceController, AActor* SourceActor);
};
