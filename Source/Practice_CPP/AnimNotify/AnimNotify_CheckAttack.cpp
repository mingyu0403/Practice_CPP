#include "AnimNotify_CheckAttack.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotify_CheckAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	FVector Pos = MeshComp->GetComponentLocation()
		+ MeshComp->GetRightVector() * Offset.X
		+ MeshComp->GetForwardVector() * Offset.Y
		+ MeshComp->GetUpVector() * Offset.Z;

	UKismetSystemLibrary::DrawDebugSphere(MeshComp->GetWorld(), Pos, Radius, 12, FLinearColor::White, 3.0f, 0.0f);

	APawn* MyPawn = Cast<APawn>(MeshComp->GetOwner());
	if (MyPawn == nullptr) return;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> IgnoreActors;
	TArray<AActor*> OutActors;

	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	IgnoreActors.Add(MeshComp->GetOwner());

	bool isOverlapped = UKismetSystemLibrary::SphereOverlapActors(MeshComp->GetWorld(), Pos, Radius, ObjectTypes, nullptr, IgnoreActors, OutActors);
	if (isOverlapped)
	{
		for (int i = 0; i < OutActors.Num(); i++)
		{
			UE_LOG(LogClass, Warning, TEXT("ApplyDamage"));
			UGameplayStatics::ApplyDamage(OutActors[i], Damage, MyPawn->GetController(), nullptr, NULL);
		}
	}
}

FString UAnimNotify_CheckAttack::GetNotifyName_Implementation() const
{
	return L"CheckAttack";
}
