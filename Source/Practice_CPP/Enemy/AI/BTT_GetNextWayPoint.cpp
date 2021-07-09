#include "BTT_GetNextWayPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../CPP_Bear.h"
#include "AIController.h"
#include "Engine/TargetPoint.h"

EBTNodeResult::Type UBTT_GetNextWayPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* MyBB = OwnerComp.GetBlackboardComponent();
	ACPP_Bear* Bear = OwnerComp.GetAIOwner()->GetPawn<ACPP_Bear>();

	int currentIndex = MyBB->GetValueAsInt(FName("WayPointIndex"));
	FVector Loc = Bear->WayPoints[currentIndex]->GetActorLocation();

	MyBB->SetValueAsVector(FName("TargetLocation"), Loc);

	int nextIndex = currentIndex + 1;
	nextIndex %= Bear->WayPoints.Num();
	MyBB->SetValueAsInt(FName("WayPointIndex"), nextIndex);

	return EBTNodeResult::Succeeded;
}
