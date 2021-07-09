// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_LookAtTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type UBTT_LookAtTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* MyBB = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	if (MyBB == nullptr) return EBTNodeResult::Failed;

	APawn* Me = OwnerComp.GetAIOwner()->GetPawn();
	if(Me == nullptr) return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(MyBB->GetValueAsObject(FName("TargetActor")));
	if(Target == nullptr) return EBTNodeResult::Failed;

	Me->SetActorRotation(
		UKismetMathLibrary::FindLookAtRotation(Me->GetActorLocation(), Target->GetActorLocation())
	);

	return EBTNodeResult::Succeeded;
}
