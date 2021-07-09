// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ChangeAttack.h"
#include "../CPP_Bear.h"
#include "AIController.h"

EBTNodeResult::Type UBTT_ChangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACPP_Bear* Bear = OwnerComp.GetAIOwner()->GetPawn<ACPP_Bear>();
	Bear->MC_SetCurrentState(EBearState::E_Attack);

	return EBTNodeResult::Succeeded;
}
