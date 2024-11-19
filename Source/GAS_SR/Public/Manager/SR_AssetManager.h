#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "SR_AssetManager.generated.h"

UCLASS()
class GAS_SR_API USR_AssetManager : public UAssetManager
{
	GENERATED_BODY()

protected:
	virtual void StartInitialLoading() override;
};
