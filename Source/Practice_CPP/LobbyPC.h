// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPC.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_CPP_API ALobbyPC : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	void UpdateNotifyMessage();
	void UpdataCurrentPlayerCountsMessage();

	void SendMessage(const FText& Text);
	
	UFUNCTION(Server, Unreliable)
	void CtoS_SendMessage(const FText& Message);

	UFUNCTION(Client, Unreliable)
	void StoC_SendMessage(const FText& Message);
};
