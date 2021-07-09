// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TitlePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_CPP_API ATitlePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UUserWidget> TitleUIClass;

	class UUserWidget* TitleUIObject;

	virtual void BeginPlay() override;

	void StartServer();
	void ConnectServer(FString IPAddress);
	
};
