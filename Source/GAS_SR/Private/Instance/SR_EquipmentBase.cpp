#include "Instance/SR_EquipmentBase.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"

ASR_EquipmentBase::ASR_EquipmentBase()
{
	PrimaryActorTick.bCanEverTick = true;

	RootSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootSphereComponent"));
	SetRootComponent(RootSphereComponent);
	RootSphereComponent->bDynamicObstacle = true;
	RootSphereComponent->SetSphereRadius(1.0f);
	RootSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	EquipMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipMeshComponent"));
	EquipMeshComponent->SetupAttachment(RootSphereComponent);
	EquipMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASR_EquipmentBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASR_EquipmentBase::ResetEquipMesh_Implementation()
{
	EquipMeshComponent->SetSkeletalMesh(nullptr);
}
