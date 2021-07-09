// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Player.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CPP_PlayerAnim.h" 
#include "Kismet/GameplayStatics.h"
#include "CPP_PlayerController.h"
#include "CPP_HUD.h" 
#include "Particles/ParticleSystemComponent.h"
#include "Components/DecalComponent.h"
#include "CPP_GhostTail.h" 
#include "Net/UnrealNetwork.h"

// Sets default values 
ACPP_Player::ACPP_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add("PLAYER");

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	Sword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);
	Gun->SetupAttachment(GetMesh(), TEXT("WEAPON_L"));
	Sword->SetupAttachment(GetMesh(), TEXT("WEAPON_R"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Character(TEXT("SkeletalMesh'/Game/PP801_P3/Meshes/Characters/Combines/SK_PP801P3_G.SK_PP801P3_G'"));
	if (SK_Character.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Character.Object);
	}
	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Gun(TEXT("StaticMesh'/Game/InsaneGun_Sword_Set/mesh/SM_Gun.SM_Gun'"));
	if (SM_Gun.Succeeded())
	{
		Gun->SetStaticMesh(SM_Gun.Object);
	}
	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Sword(TEXT("StaticMesh'/Game/InsaneGun_Sword_Set/mesh/SM_Sword.SM_Sword'"));
	if (SM_Sword.Succeeded())
	{
		Sword->SetStaticMesh(SM_Sword.Object);
	}
	ConstructorHelpers::FClassFinder<UAnimInstance> ABP_Anim(TEXT("AnimBlueprint'/Game/Blueprints/Player/Anim/ABP_Player.ABP_Player_C'"));
	if (ABP_Anim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ABP_Anim.Class);
	}
	ConstructorHelpers::FObjectFinder<UParticleSystem> PS_Laser(TEXT("ParticleSystem'/Game/Effects/LaserParticle.LaserParticle'"));
	if (PS_Laser.Succeeded())
	{
		LaserParticle = PS_Laser.Object;
	}
	ConstructorHelpers::FObjectFinder<UParticleSystem> PS_Blood(TEXT("ParticleSystem'/Game/Effects/P_BloodEffect.P_BloodEffect'"));
	if (PS_Blood.Succeeded())
	{
		BloodParticle = PS_Blood.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_HitDecal(TEXT("MaterialInstanceConstant'/Game/Effects/Decal/M_decal_Inst.M_decal_Inst'"));
	if (MI_HitDecal.Succeeded())
	{
		HitDecal = MI_HitDecal.Object;
	}

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	SpringArm->TargetArmLength = 500.0f;

	bUseControllerRotationYaw = false;
	SpringArm->bUsePawnControlRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void ACPP_Player::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHP = MaxHP;

	ACPP_HUD* HUD = Cast<ACPP_HUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	HUD->SetHP(MaxHP, CurrentHP);
}

// Called every frame
void ACPP_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled())
	{
		CtoS_SetAimPitch();
	}
}

// Called to bind functionality to input
void ACPP_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &ACPP_Player::OnTurn);
	PlayerInputComponent->BindAxis("LookUp", this, &ACPP_Player::OnLookUp);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACPP_Player::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPP_Player::OnMoveRight);
	
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &ACPP_Player::OnAttack);
	PlayerInputComponent->BindAction("IronSight", EInputEvent::IE_Pressed, this, &ACPP_Player::OnIronSight_Press);
	PlayerInputComponent->BindAction("IronSight", EInputEvent::IE_Released, this, &ACPP_Player::OnIronSight_Release);
	PlayerInputComponent->BindAction("Rolling", EInputEvent::IE_Released, this, &ACPP_Player::CtoS_Rolling);
}

void ACPP_Player::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimBP = Cast<UCPP_PlayerAnim>(GetMesh()->GetAnimInstance());
}

void ACPP_Player::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPP_Player, bIsIronSight);
	DOREPLIFETIME(ACPP_Player, AimPitch); 
	DOREPLIFETIME(ACPP_Player, CurrentHP);
}

void ACPP_Player::OnTurn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void ACPP_Player::OnLookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void ACPP_Player::OnMoveForward(float AxisValue)
{
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
	AddMovementInput(ForwardVector, AxisValue);
}

void ACPP_Player::OnMoveRight(float AxisValue)
{
	FVector RightVector = UKismetMathLibrary::GetRightVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
	AddMovementInput(RightVector, AxisValue);
}

void ACPP_Player::OnAttack()
{
	if (bIsIronSight == true)
	{
		CtoS_GunAttack();
	}
	else
	{
		CtoS_SwordAttack();
	}
}

void ACPP_Player::OnIronSight_Press()
{
	ACPP_PlayerController* PC = Cast<ACPP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		ACPP_HUD* HUD = Cast<ACPP_HUD>(PC->GetHUD());
		if (HUD)
		{
			HUD->ShowCrossHair(true);
		}
	}
	CtoS_SetIronSight(true);
}

void ACPP_Player::OnIronSight_Release()
{
	ACPP_PlayerController* PC = Cast<ACPP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		ACPP_HUD* HUD = Cast<ACPP_HUD>(PC->GetHUD());
		if (HUD)
		{
			HUD->ShowCrossHair(false);
		}
	}
	CtoS_SetIronSight(false);
}

void ACPP_Player::CtoS_Rolling_Implementation()
{
	MC_Rolling(GetLastMovementInputVector());
}

void ACPP_Player::MC_Rolling_Implementation(FVector LastInputVector)
{
	if (AnimBP)
	{
		if (LastInputVector.SizeSquared() > 0)
		{
			SetActorRotation(UKismetMathLibrary::MakeRotationFromAxes(LastInputVector, FVector::ZeroVector, FVector::ZeroVector));
			bUseControllerRotationYaw = false;
		}

		AnimBP->Rolling();
	}
}

void ACPP_Player::CtoS_SwordAttack_Implementation()
{
	MC_SwordAttack();
}

void ACPP_Player::MC_SwordAttack_Implementation()
{
	if (AnimBP)
	{
		AnimBP->SwordAttack();
	}
}

void ACPP_Player::CtoS_GunAttack_Implementation()
{
	if (AnimBP)
	{
		if (AnimBP->bIsShotMotion == false)
		{
			FVector TargetLoc = CheckTargetAtCamera();
			FVector MuzzleLoc = GetMesh()->GetSocketLocation("Muzzle");

			MC_GunAttack(TargetLoc, MuzzleLoc);
		}
	}
}

void ACPP_Player::MC_GunAttack_Implementation(FVector TargetLoc, FVector MuzzleLoc)
{
	SpawnGunEffect(MuzzleLoc, TargetLoc);
	AnimBP->GunAttack();
}

void ACPP_Player::CtoS_SetIronSight_Implementation(bool IsOn)
{
	MC_SetIronSight(IsOn);
}

void ACPP_Player::MC_SetIronSight_Implementation(bool IsOn)
{
	bIsIronSight = IsOn;
	bUseControllerRotationYaw = IsOn;

	GetCharacterMovement()->MaxWalkSpeed = bIsIronSight == true ? AimWalkSpeed : NormalWalkSpeed;
}

void ACPP_Player::SpawnGhostTail()
{
	ACPP_GhostTail* Ghost = GetWorld()->SpawnActor<ACPP_GhostTail>(ACPP_GhostTail::StaticClass(), GetMesh()->GetComponentTransform());
	Ghost->Init(GetMesh());
}

void ACPP_Player::CtoS_CheckHP_Implementation(float HP)
{
	MC_CheckHP(HP);
}

void ACPP_Player::MC_CheckHP_Implementation(float HP)
{
	CurrentHP = FMath::Clamp(HP, 0.0f, MaxHP);
	
	if (IsLocallyControlled())
	{
		ACPP_PlayerController* PC = Cast<ACPP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PC)
		{
			ACPP_HUD* HUD = Cast<ACPP_HUD>(PC->GetHUD());
			if (HUD)
			{
				HUD->SetHP(MaxHP, CurrentHP);
			}
		}
	}

	if (CurrentHP <= 0.0f)
	{
		AnimBP->Dead();
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}
}

float ACPP_Player::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CtoS_CheckHP(CurrentHP - Damage);

	UE_LOG(LogClass, Warning, TEXT("Damage = %f, CurrentHP = %f"), Damage, CurrentHP);

	return Damage;
}

void ACPP_Player::CtoS_SetAimPitch_Implementation()
{
	float NewAimPitch = -1 * GetControlRotation().Pitch;
	MC_SetAimPitch(NewAimPitch);
}

void ACPP_Player::MC_SetAimPitch_Implementation(float NewAimPitch)
{
	AimPitch = NewAimPitch;
}

void ACPP_Player::RollingEnd()
{
	MC_SetIronSight(bIsIronSight);
}

FVector ACPP_Player::CheckTargetAtCamera()
{
	FVector CameraLoc = Camera->GetComponentLocation();
	FVector CameraForward = Camera->GetForwardVector();

	FVector StartLoc = CameraLoc; // 레이저 시작 지점.
	FVector EndLoc = CameraLoc + (CameraForward * 5000.0f); // 레이저 끝나는 지점.

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // 히트 가능한 오브젝트 유형들.
	TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	TEnumAsByte<EObjectTypeQuery> WorldDynamic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic);
	ObjectTypes.Add(WorldStatic);
	ObjectTypes.Add(WorldDynamic);

	TArray<AActor*> IgnoreActors; // 무시할 액터들.

	FHitResult HitResult; // 히트 결과 값 받을 변수.

	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		StartLoc,
		EndLoc,
		ObjectTypes,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		HitResult,
		true
		// 여기 밑에 3개는 기본 값으로 제공됨. 바꾸려면 적으면 됨.
		//, FLinearColor::Red
		//, FLinearColor::Green
		//, 5.0f
	);

	return Result == true ? HitResult.ImpactPoint : HitResult.TraceEnd;
}

void ACPP_Player::SpawnGunEffect(FVector& MuzzleLoc, FVector& TargetLoc)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> IgnoreActors;
	FHitResult HitResult;

	FVector EndLoc = MuzzleLoc + 
		UKismetMathLibrary::GetDirectionUnitVector(MuzzleLoc, TargetLoc) * (UKismetMathLibrary::Vector_Distance(MuzzleLoc, TargetLoc) + 1);

	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		MuzzleLoc,
		EndLoc,
		ObjectTypes,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	if (Result)
	{

	}

	FVector BeamStartLoc = HitResult.TraceStart;
	FVector BeamEndLoc;
	if (Result == false)
	{
		BeamEndLoc = HitResult.TraceEnd;
	}
	else 
	{
		BeamEndLoc = HitResult.ImpactPoint;
		SpawnHitEffect(HitResult);

		UGameplayStatics::ApplyPointDamage(
			HitResult.GetActor(),
			30.0f,
			HitResult.GetActor()->GetActorLocation(),
			HitResult,
			nullptr,
			this,
			nullptr
		);
	}

	UParticleSystemComponent* LaserParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaserParticle, BeamStartLoc);
	LaserParticleComponent->SetBeamEndPoint(0, BeamEndLoc);
}

void ACPP_Player::SpawnHitEffect(FHitResult& HitResult)
{
	ACharacter* Character = Cast<ACharacter>(HitResult.GetActor());
	if (Character)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			BloodParticle,
			HitResult.ImpactPoint
		);
	}
	else
	{
		UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			HitDecal,
			FVector(16.0f),
			HitResult.ImpactPoint,
			UKismetMathLibrary::MakeRotationFromAxes(HitResult.ImpactNormal, FVector::ZeroVector, FVector::ZeroVector),
			5.0f
		);
		DecalComponent->SetFadeScreenSize(0.0f); // 멀리서 봤을 때, 데칼이 작게 보여도 보이게 하는 설정.
	}
}
