// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGM.h"
#include "LobbyGS.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void ALobbyGM::BeginPlay()
{
	Super::BeginPlay();

	ALobbyGS* GS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (nullptr == GS) return;

	GS->InitCountDownNum();
	GetWorldTimerManager().SetTimer(
		CountDownTimer, this, &ALobbyGM::StartTimerCountDown, 1.0f, true
	);
}

void ALobbyGM::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ALobbyGS* _GS = GetGameState<ALobbyGS>();
	if (_GS == nullptr) return;

	_GS->SetCurrentPlayerCounts(CountPlayer());
}

void ALobbyGM::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	ALobbyGS* _GS = GetGameState<ALobbyGS>();
	if (_GS == nullptr) return;

	_GS->SetCurrentPlayerCounts(CountPlayer(Exiting));
}

int32 ALobbyGM::CountPlayer(AController* Ignore)
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), OutActors);

	if (Ignore != nullptr)
	{
		APlayerController* _IgnorePC = Cast<APlayerController>(Ignore);
		OutActors.Remove(_IgnorePC);
	}

	return OutActors.Num();
}

void ALobbyGM::StartTimerCountDown()
{
	//ALobbyGS* GS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetWorld()));
	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (nullptr == GS) return;

	bool bIsGreaterThanZero = GS->CountDown();
	if (bIsGreaterThanZero == false)
	{
		GetWorldTimerManager().ClearTimer(CountDownTimer);
	}
}

void ALobbyGM::StartGame()
{
	if (HasAuthority())
	{
		GetWorld()->ServerTravel(TEXT("Level_0"));
	}
}
