// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "../CPP_Bear.h"
#include "BTT_CheckDistance.generated.h"

UENUM()
enum class EOperator : uint8
{
	E_Equal				UMETA(DisplayName = "=="),
	E_Greater			UMETA(DisplayName = "<"),
	E_GreaterOrEqual	UMETA(DisplayName = "<="),
	E_Less				UMETA(DisplayName = ">"),
	E_LessOrEqual		UMETA(DisplayName = ">=")
};

UCLASS()
class PRACTICE_CPP_API UBTT_CheckDistance : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Distance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EOperator Operator;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EBearState BeChangedState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsSuccess;
};
