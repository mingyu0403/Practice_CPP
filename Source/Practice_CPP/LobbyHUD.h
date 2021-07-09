// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_CPP_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	void CreateLobbyWidget();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UUserWidget> LobbyUIClass;
	class ULobbyUI* LobbyUIObject;

	void SetNoticeMessage(int32 CountTime);
	void SetCurrentPlayerCountsMessage(int32 Counts);
	void AddChatMessage(FText Text);

	void ShowStartAnim();
};
