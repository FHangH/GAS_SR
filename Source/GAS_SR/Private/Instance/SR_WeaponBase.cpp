#include "Instance/SR_WeaponBase.h"
#include "Components/CapsuleComponent.h"

ASR_WeaponBase::ASR_WeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerBoxComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TriggerBoxComponent"));
	TriggerBoxComponent->SetupAttachment(EquipMeshComponent);
}

void ASR_WeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASR_WeaponBase::BeginWeaponAttack_Implementation(const FGameplayTag& Tag)
{
}

void ASR_WeaponBase::EndWeaponAttack_Implementation()
{
}
