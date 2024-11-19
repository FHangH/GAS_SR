#include "GAS_SR/Public/Gameplay/Character/SR_HeroBase.h"

ASR_HeroBase::ASR_HeroBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASR_HeroBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASR_HeroBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASR_HeroBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}