#pragma once

#include "CoreMinimal.h"
#include "SR_CharacterBase.h"
#include "SR_HeroBase.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class GAS_SR_API ASR_HeroBase : public ASR_CharacterBase
{
	GENERATED_BODY()
	
	/* Property */
protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category="A_SR|Hero")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category="A_SR|Hero")
	UCameraComponent* CameraComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="A_SR|Hero|Camera")
	float DefaultArmLength { 1000.f };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="A_SR|Hero|Camera")
	float DefaultCameraFOV { 90.f };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="A_SR|Hero|Camera")
	FVector DefaultSourceOffset { 0.f, 0.f, 0.f };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="A_SR|Hero|Camera")
	FVector DefaultTargetOffset { 0.f, 0.f, 0.f };
	
	bool IsHasBindInput { false };
	
	/* Function */
public:
	explicit ASR_HeroBase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnRep_PlayerState() override;
	
	void BindASComponentInput(UInputComponent* PlayerInputComponent);
};
