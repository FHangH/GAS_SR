#include "GAS_SR/Public/Manager/SR_AssetManager.h"
#include "AbilitySystemGlobals.h"

void USR_AssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
