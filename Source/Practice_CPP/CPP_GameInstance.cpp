#include "CPP_GameInstance.h"
#include "Kismet/KismetMathLibrary.h"

void UCPP_GameInstance::SetUserName(FString Name)
{
	UserName = Name;
}

FString UCPP_GameInstance::GetUserName()
{
	if (UserName.IsEmpty())
	{
		int RandomNumber = UKismetMathLibrary::RandomIntegerInRange(0, 9999);
		FString TempUserName = FString::Printf(TEXT("Guest_%d"), RandomNumber);
		UserName = TempUserName;
	}

	return UserName;
}
