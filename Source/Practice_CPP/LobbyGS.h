// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGS.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_CPP_API ALobbyGS : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(ReplicatedUsing = OnRep_CountDownNum)
	int32 CountDownNum;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentPlayerCounts)
	int32 CurrentPlayerCounts;

	void InitCountDownNum();
	bool CountDown();

	void SetCountDownNum(int32 Num);
	void SetCurrentPlayerCounts(int32 Counts);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_CountDownNum();
	UFUNCTION()
	void OnRep_CurrentPlayerCounts();
};