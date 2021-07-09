// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_CheckTargetDistance.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../CPP_Bear.h"


void UBTS_CheckTargetDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* MyBB = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	if (MyBB == nullptr) return;

	AActor* Target = Cast<AActor>(MyBB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (Target == nullptr) return;

	ACPP_Bear* Me = OwnerComp.GetAIOwner()->GetPawn<ACPP_Bear>();
	if (Me == nullptr) return;

	float distanceSquared = Distance * Distance;
	float currentDistanceSquared = FVector::DistSquared(Target->GetActorLocation(), Me->GetActorLocation());
	if (distanceSquared < currentDistanceSquared)
	{
		Me->MC_SetCurrentState(EBearState::E_Roam);
		MyBB->SetValueAsObject(TargetActorKey.SelectedKeyName, nullptr);
	}
}
