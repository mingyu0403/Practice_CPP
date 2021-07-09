// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CPP_BearAnim.generated.h"

class ACPP_Bear;

UENUM(BlueprintType)
enum class EBearState : uint8
{
	E_Roam,
	E_Chase,
	E_Attack,
	E_Dead,
};

UCLASS()
class PRACTICE_CPP_API UCPP_BearAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(VisibleAnywhere)
	ACPP_Bear* Bear;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "state")
	float Speed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "state")
	EBearState CurrentState;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "state")
	bool bIsAttackTrigger;

public:
	UFUNCTION()
	void AnimNotify_AttackRelease();

	void SetIsAttackTrigger(bool isOn);
};
