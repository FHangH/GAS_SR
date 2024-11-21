#include "GAS_SR/Public/Gameplay/Character/SR_HeroBase.h"
#include "Camera/CameraComponent.h"
#include "Data/SR_Types.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gameplay/GAS/SR_AbilitySystemComponent.h"

ASR_HeroBase::ASR_HeroBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->SetWorldRotation(FRotator(-45.f, 0.f, 0.f));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ASR_HeroBase::BeginPlay()
{
	Super::BeginPlay();

	SpringArmComponent->TargetArmLength = DefaultArmLength;
	SpringArmComponent->SocketOffset = DefaultSourceOffset;
	SpringArmComponent->TargetOffset = DefaultTargetOffset;
	CameraComponent->SetFieldOfView(DefaultCameraFOV);
}

void ASR_HeroBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ASR_HeroBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	BindASComponentInput(PlayerInputComponent);
}

void ASR_HeroBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	BindASComponentInput(InputComponent);
}

void ASR_HeroBase::BindASComponentInput(UInputComponent* PlayerInputComponent)
{
	if (!IsHasBindInput && AbilitySystemComponent && PlayerInputComponent)
	{
		const FGameplayAbilityInputBinds AbilityInputBinds
		{
			"Confirm",
			"Cancel",
			"ESR_AbilityInputID",
			static_cast<int32>(ESR_AbilityInputID::Confirm),
			static_cast<int32>(ESR_AbilityInputID::Cancel)
		};
		AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, AbilityInputBinds);
		IsHasBindInput = true;		
	}
}
