#include "BTT_CheckDistance.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

EBTNodeResult::Type UBTT_CheckDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* MyBB = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	if (MyBB == nullptr) return EBTNodeResult::Failed;

	APawn* Target = Cast<APawn>(MyBB->GetValueAsObject(FName("TargetActor")));
	if (Target == nullptr) return EBTNodeResult::Failed;

	ACPP_Bear* Bear = OwnerComp.GetAIOwner()->GetPawn<ACPP_Bear>();
	if (Bear == nullptr) return EBTNodeResult::Failed;

	FVector TargetLoc = Target->GetActorLocation();
	FVector MyLoc = Bear->GetActorLocation();

	float CurrentDistanceSquare = FVector::DistSquared(TargetLoc, MyLoc);
	float DistanceSquare = Distance * Distance;

	bool bCondition = false;
	switch (Operator)
	{
	case EOperator::E_Equal:			bCondition = DistanceSquare == CurrentDistanceSquare; break;
	case EOperator::E_Greater:			bCondition = DistanceSquare < CurrentDistanceSquare; break; 
	case EOperator::E_GreaterOrEqual:	bCondition = DistanceSquare <= CurrentDistanceSquare; break; 
	case EOperator::E_Less:				bCondition = DistanceSquare > CurrentDistanceSquare; break; 
	case EOperator::E_LessOrEqual:		bCondition = DistanceSquare >= CurrentDistanceSquare; break; 
	}

	if (bCondition)
	{
		Bear->MC_SetCurrentState(BeChangedState);
		return bIsSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return bIsSuccess ? EBTNodeResult::Failed : EBTNodeResult::Succeeded;
}