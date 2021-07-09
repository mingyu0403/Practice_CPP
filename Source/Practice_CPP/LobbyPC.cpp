// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPC.h"
#include "LobbyGS.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyHUD.h"
#include "LobbyUI.h"
#include "CPP_GameInstance.h"
#include "Engine/World.h"

void ALobbyPC::Tick(float DeltaSeconds)
{

}

void ALobbyPC::BeginPlay()
{
	if (HasAuthority())
	{
		ALobbyHUD* _HUD = GetHUD<ALobbyHUD>();
		if (_HUD)
		{
			_HUD->ShowStartAnim();
		}
	}
}

void ALobbyPC::UpdateNotifyMessage()
{
	ALobbyGS* _GS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetWorld()));
	ALobbyHUD* _HUD = GetHUD<ALobbyHUD>();
	if (_GS && _HUD)
	{
		_HUD->SetNoticeMessage(_GS->CountDownNum);
	}
}

void ALobbyPC::UpdataCurrentPlayerCountsMessage()
{
	ALobbyGS* _GS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetWorld()));
	ALobbyHUD* _HUD = GetHUD<ALobbyHUD>();
	if (_GS && _HUD)
	{
		_HUD->SetCurrentPlayerCountsMessage(_GS->CurrentPlayerCounts);
	}
}

void ALobbyPC::SendMessage(const FText& Text)
{
	UCPP_GameInstance* _GI = GetGameInstance<UCPP_GameInstance>();
	if (_GI)
	{
		FString UserName = _GI->GetUserName();
		FString Message = FString::Printf(TEXT("%s : %s"), *UserName, *Text.ToString());

		CtoS_SendMessage(FText::FromString(Message));
	}
}

void ALobbyPC::CtoS_SendMessage_Implementation(const FText& Message)
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetPawn()->GetWorld(), APlayerController::StaticClass(), OutActors);
	for (AActor* OutActor : OutActors)
	{
		ALobbyPC* _PC = Cast<ALobbyPC>(OutActor);
		_PC->StoC_SendMessage(Message);
	}
}

void ALobbyPC::StoC_SendMessage_Implementation(const FText& Message)
{
	ALobbyHUD* _HUD = GetHUD<ALobbyHUD>();
	if (_HUD == nullptr) return;

	_HUD->AddChatMessage(Message);
}
