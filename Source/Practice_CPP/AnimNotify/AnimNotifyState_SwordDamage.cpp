#include "AnimNotifyState_SwordDamage.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UAnimNotifyState_SwordDamage::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	IgnoreActor.Empty();
	IgnoreActor.Add(MeshComp->GetOwner()); // 자신 무시.
}

void UAnimNotifyState_SwordDamage::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	FHitResult HitResult;

	bool Result = UKismetSystemLibrary::SphereTraceSingleForObjects(
		MeshComp->GetWorld(),
		MeshComp->GetSocketLocation(FName("Trail_Start")),
		MeshComp->GetSocketLocation(FName("Trail_End")),
		20.0f,
		ObjectTypes,
		false,
		IgnoreActor,
		EDrawDebugTrace::ForDuration,
		HitResult,
		false,
		FLinearColor::Red,
		FLinearColor::Green,
		3.0f
	);

	if (Result)
	{
		IgnoreActor.Add(HitResult.GetActor());
		UGameplayStatics::ApplyDamage(HitResult.GetActor(), Damage, nullptr, MeshComp->GetOwner(), NULL);
	}
}
