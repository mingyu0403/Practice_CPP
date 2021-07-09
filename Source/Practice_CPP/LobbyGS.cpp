// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGS.h"
#include "Net/UnrealNetwork.h"
#include "LobbyPC.h"

#include "Kismet/GameplayStatics.h"

void ALobbyGS::InitCountDownNum()
{
	SetCountDownNum(30);
}

bool ALobbyGS::CountDown()
{
	SetCountDownNum(CountDownNum - 1);

	return CountDownNum > 0;
}

void ALobbyGS::SetCountDownNum(int32 Num)
{
	CountDownNum = Num;
	// 서버가 아닌 다른 곳에서 호출 될 가능성이 있다면, if (HasAuthority()) 검사해야 함.
	OnRep_CountDownNum(); // Replicate할 때, 정작 자신은 이벤트를 못 받음.블루프린트에서는 해주는데, C++에서는 강제로 이렇게 호출해줘야 함.
}

void ALobbyGS::SetCurrentPlayerCounts(int32 Counts)
{
	CurrentPlayerCounts = Counts;
	OnRep_CurrentPlayerCounts();
}

void ALobbyGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGS, CountDownNum);
	DOREPLIFETIME(ALobbyGS, CurrentPlayerCounts);
}

void ALobbyGS::OnRep_CountDownNum()
{
	ALobbyPC* _PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (_PC)
	{
		_PC->UpdateNotifyMessage();
	}
}

void ALobbyGS::OnRep_CurrentPlayerCounts()
{
	ALobbyPC* _PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (_PC)
	{
		_PC->UpdataCurrentPlayerCountsMessage();
	}
}
