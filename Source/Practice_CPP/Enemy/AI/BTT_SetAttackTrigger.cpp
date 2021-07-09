#include "BTT_SetAttackTrigger.h"

#include "AIController.h"
#include "../CPP_Bear.h"

EBTNodeResult::Type UBTT_SetAttackTrigger::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACPP_Bear* Bear = OwnerComp.GetAIOwner()->GetPawn<ACPP_Bear>();
	if (Bear == nullptr) return EBTNodeResult::Failed;

	Bear->MC_SetAttackTrigger(true);

	return EBTNodeResult::Succeeded;
}
