#include "TitlePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ATitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (false == IsLocalPlayerController()) return;

	if (TitleUIClass)
	{
		TitleUIObject = CreateWidget<UUserWidget>(this, TitleUIClass);
		if (TitleUIObject)
		{
			TitleUIObject->AddToViewport();
		}
	}
	SetShowMouseCursor(false);
	SetInputMode(FInputModeUIOnly());
}

void ATitlePlayerController::StartServer()
{
	/*
	UCPP_GameInstance* MyGameInstance = Cast<UCPP_GameInstance>(GetGameInstance());
	if (nullptr == MyGameInstance) return;
	UTitileUI* MyUI = Cast<UTitileUI>(GetHUD());
	if (nullptr == MyUI) return;

	MyGameInstance->SetUserName(MyUI->GetUserNameText());
	*/
	UGameplayStatics::OpenLevel(GetWorld(), FName("Lobby"), true, ((FString)(L"Listen")));
}

void ATitlePlayerController::ConnectServer(FString IPAddress)
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(IPAddress));
}