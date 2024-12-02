#pragma once

#include "CoreMinimal.h"
#include "SR_Types.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(SRLog,Log,All);

#define SRLOG(lg) UE_LOG(SRLog,Log,lg);
#define SRWARNING(lg) UE_LOG(SRLog,Warning,lg)
#define SRWARNING1(lg,ex1) UE_LOG(SRLog,Warning,lg,ex1)
#define SRLOGEX1(lg,ex1) UE_LOG(SRLog,Log,lg,ex1)
#define SRLOGEX2(lg,ex1,ex2) UE_LOG(SRLog,Log,lg,ex1,ex2)
#define SRLOGEX3(lg,ex1,ex2,ex3) UE_LOG(SRLog,Log,lg,ex1,ex2,ex3)
#define SRLOGEX4(lg,ex1,ex2,ex3,ex4) UE_LOG(SRLog,Log,lg,ex1,ex2,ex3,ex4)
#define SRLOGEX5(lg,ex1,ex2,ex3,ex4,ex5) UE_LOG(SRLog,Log,lg,ex1,ex2,ex3,ex4,ex5)

UENUM(BlueprintType)
enum class ESR_AbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	AbilityMain,
	AbilitySecondary,
	Ability1,
	Ability2,
};

UENUM(BlueprintType)
enum class ESR_InputMode : uint8
{
	OnlyUI,
	OnlyGame,
	Both
};

//可交互物体的类型
UENUM(BlueprintType)
enum class ESRInteractableObjType : uint8
{
	None,
	//角色
	Role,
	//可拾取道具
	PickUpProp,
	//查看类道具
	ViewableProp,
	Other
};
//可交互物体的阵营
UENUM(BlueprintType)
enum class ESRInteractableObjCamp : uint8
{
	None,
	Hero,
	Friend,
	Monster,
	Prop,
	Custom
};
//交互类型
UENUM(BlueprintType)
enum class ESRInteractType : uint8
{
	None,
	Attack,
	Touch,
	Custom
};


UENUM(BlueprintType)
enum class ESRAbilityInputID : uint8 
{
	None,
	Confirm,
	Cancel,
	AbilityMain,
	AbilitySecondary,
	Ability1,
	Ability2,
};

UENUM(BlueprintType)
enum class ESRDamageStatus : uint8
{
	Normal,
	Crit,
	Realdamage

};

UENUM(BlueprintType)
enum class ESRDamageType : uint8
{
	None,
	Physics,
	Fire,
	Ice,
	Poison,
	Electricity,
	Holy,
	Arcane

};

/*
* 伤害参数
*/
USTRUCT(BlueprintType)
struct FSRDamageParameter
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_SR|Damage")
	FHitResult HitResult;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_SR|Damage")
	float UnmitigatedDamage = 0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_SR|Damage")
	float MitigatedDamage = 0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_SR|Damage")
	ESRDamageType Type;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_SR|Damage")
	ESRDamageStatus Status;
};

/*
* 格挡参数
*/
USTRUCT(BlueprintType)
struct FSRBlockParameter
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_SR|Block")
	FHitResult HitResult;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_SR|Block")
	float UnmitigatedDamage = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_SR|Block")
	float MitigatedDamage = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_SR|Block")
	float BlockValue = 0;
};

/*
* 闪避参数
*/
USTRUCT(BlueprintType)
struct FSRDodgeParameter
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_SR|Dodge")
	FHitResult HitResult;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_SR|Dodge")
	FString ExtraInfo;
};
