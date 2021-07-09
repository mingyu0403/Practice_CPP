// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CheckTargetDistance.generated.h"

UCLASS()
class PRACTICE_CPP_API UBTS_CheckTargetDistance : public UBTService
{
	GENERATED_BODY()
	
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	struct FBlackboardKeySelector TargetActorKey;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Distance;
};
