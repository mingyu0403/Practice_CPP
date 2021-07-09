#include "CPP_Bear.h"
#include "CPP_BearAnim.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/PawnSensingComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "../UI/CWidget_BasicHPProgress.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ACPP_Bear::ACPP_Bear()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FClassFinder<UAnimInstance> ABP_Anim(TEXT("AnimBlueprint'/Game/Blueprints/Enemy/Anim/BP_BearAnim.BP_BearAnim_C'"));
	if (ABP_Anim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ABP_Anim.Class);
	}

	GetCapsuleComponent()->SetCapsuleRadius(100);
	GetCapsuleComponent()->SetCapsuleHalfHeight(100);
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PAWNSENSING"));
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WIDGET"));

	Widget->SetupAttachment(RootComponent);

	ConstructorHelpers::FClassFinder<UCWidget_BasicHPProgress> UW_HP_Progress(TEXT("WidgetBlueprint'/Game/Blueprints/UI/WG_HPProgress.WG_HPProgress_C'"));
	if (UW_HP_Progress.Succeeded())
	{
		Widget->SetWidgetClass(UW_HP_Progress.Class);
	}
	Widget->SetDrawSize(FVector2D(300.0f, 30.0f));
	Widget->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
}

void ACPP_Bear::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHP;
	SetHPProgress();
}

void ACPP_Bear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	BillboardUI();
}

void ACPP_Bear::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimBP = Cast<UCPP_BearAnim>(GetMesh()->GetAnimInstance());
	PawnSensing->OnSeePawn.AddDynamic(this, &ACPP_Bear::OnSeePawn);
}

float ACPP_Bear::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (HasAuthority())
	{
		if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
		{
			const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);

			UE_LOG(LogClass, Warning, TEXT("BoneName : %s"), *(PointDamageEvent->HitInfo.BoneName.ToString()));

			if (0 == (PointDamageEvent->HitInfo.BoneName).Compare(FName(TEXT("Head"))))
			{
				Damage *= 5;
			}
		}

		MC_CheckHP(CurrentHP - Damage);
		UE_LOG(LogClass, Warning, TEXT("Damage : %f, CurrentHP : %f"), Damage, CurrentHP);

		SetTarget(DamageCauser);
	}

	return Damage;
}

void ACPP_Bear::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPP_Bear, CurrentState);
	DOREPLIFETIME(ACPP_Bear, CurrentHP);
}

void ACPP_Bear::SetTarget(AActor* Actor)
{
	UBlackboardComponent* MyBB = UAIBlueprintHelperLibrary::GetBlackboard(this);
	if (MyBB == nullptr) return;

	if (MyBB->GetValueAsObject(FName("TargetActor")) == nullptr)
	{
		MyBB->SetValueAsObject(FName("TargetActor"), (UObject*)Actor);
		MC_SetCurrentState(EBearState::E_Chase);
	}
}

void ACPP_Bear::MC_SetCurrentState_Implementation(EBearState state)
{
	if (CurrentState == EBearState::E_Dead) return;

	// 여기서 동기화하고, 블랙보드 값을 매겨야 한다.
	CurrentState = state;
	if (AnimBP)
	{
		AnimBP->CurrentState = state;
	}

	// 서버에만 블랙보드가 있기 때문에, 
	UBlackboardComponent* MyBB = UAIBlueprintHelperLibrary::GetBlackboard(this);
	if (MyBB == nullptr) return; // 클라이언트들은 여기서 retrun이 된다.

	MyBB->SetValueAsEnum(FName("CurrentState"), (uint8)state);

	// 그냥 이렇게 짜면 될듯.
	//if (MyBB)
	//{
	//	MyBB->SetValueAsEnum(FName("CurrentState"), (uint8)state);
	//}

}

void ACPP_Bear::MC_CheckHP_Implementation(float HP)
{
	CurrentHP = FMath::Clamp(HP, 0.0f, MaxHP);
	SetHPProgress();

	if (CurrentHP <= 0.0f)
	{
		MC_SetCurrentState(EBearState::E_Dead);
	}
}

void ACPP_Bear::BillboardUI()
{
	FVector Start = Widget->GetOwner()->GetActorLocation();
	FVector Target = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
	
	FRotator RotUI = UKismetMathLibrary::FindLookAtRotation(Start, Target);
	Widget->SetWorldRotation(RotUI);
}

void ACPP_Bear::SetHPProgress()
{
	UCWidget_BasicHPProgress* HPBar = Cast<UCWidget_BasicHPProgress>(Widget->GetWidget());
	
	if (HPBar)
	{
		HPBar->SetHPProgress(CurrentHP / MaxHP);
		HPBar->SetHPLabel(FText::FromString(FString::Printf(TEXT("%.1f / %.1f"), CurrentHP, MaxHP)));
	}
}

void ACPP_Bear::MC_SetAttackTrigger_Implementation(bool isOn) 
{
	if (AnimBP)
	{
		AnimBP->SetIsAttackTrigger(isOn);
	}
}

void ACPP_Bear::OnSeePawn(APawn* Pawn)
{
	if (Pawn->ActorHasTag("PLAYER"))
	{
		SetTarget(Pawn);
	}
}
