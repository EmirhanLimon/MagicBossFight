
#include "NightShade.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ANightShade::ANightShade() :
bCombatRange(false),
ClampValue(0.f),
Alpha(0.f),
Distance(0.f),
bCosmicSphereRange(false),
NightShadeHealth(750.f),
bCanSpawnLich(false),
bSpawnedLich(false),
i(0),
bCanSpell(true)
{
	PrimaryActorTick.bCanEverTick = true;

	NightShadePawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("NightPawnSensing"));

	IceShotNiagaraEffectComponent = CreateDefaultSubobject<USceneComponent>(TEXT("NiagaraEffectComponent"));
	IceShotNiagaraEffectComponent->SetupAttachment(GetRootComponent());
}

void ANightShade::BeginPlay()
{
	Super::BeginPlay();
	AIC_Ref = Cast<AEnemyController>(GetController());
	Character = Cast<AMagicCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	NightShadePawnSensing->OnHearNoise.AddDynamic(this, &ANightShade::OnHearNoise);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCharacterMovement()->MaxWalkSpeed = 300;
	FTimerHandle SpawnLichTimer;
	GetWorldTimerManager().SetTimer(SpawnLichTimer, this, &ANightShade::CanSpawnLich, 2.f);
	if(Character)
	{
		Character->NightShadeRef(this);
	}
}

void ANightShade::OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume)
{
	if(OtherActor == nullptr) return;
	Character = Cast<AMagicCharacter>(OtherActor);
	if(Character && !bCombatRange && CombatState == ECombatState::ECS_Unoccupied)
	{
		if(!IsValid(AIC_Ref))
		{
			AIC_Ref = Cast<AEnemyController>(GetController());
		}
		else
		{
			AIC_Ref->MoveToLocation(Character->GetActorLocation(), -1.f);
		}
	}
}

void ANightShade::RotateToCharacter()
{
	if(bCombatRange || CombatState == ECombatState::ECS_FireTimerInProgress)
	{
		ClampValue =  FMath::Clamp((Alpha + 0.4f),0,1);
		Alpha = ClampValue;
		FVector TargetLocation {Character->GetActorLocation().X,Character->GetActorLocation().Y,Character->GetActorLocation().Z * 2.f};
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),TargetLocation);
		FRotator NewRotation = FRotator(FQuat::Slerp(UE::Math::TQuat<double>(GetActorRotation()), UE::Math::TQuat<double>(TargetRotation), Alpha));
		SetActorRotation(NewRotation);
	}
}

void ANightShade::CalculateDistance()
{
	Distance = FVector::Distance(GetActorLocation(),Character->GetActorLocation());
	if(Distance >= 1500.f)
	{
		bCombatRange = false;
	}
	else
	{
		bCombatRange = true;
		if(!IsValid(AIC_Ref))
		{
			AIC_Ref = Cast<AEnemyController>(GetController());
		}
		else
		{
			AIC_Ref->StopMovement();
			if(CombatState == ECombatState::ECS_Unoccupied && bCanSpell)
			{
				UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				if(AnimInstance && SpellMontage)
				{
					bCanSpell = false;
					CombatState = ECombatState::ECS_FireTimerInProgress;
					AnimInstance->Montage_Play(SpellMontage);
					AnimInstance->Montage_JumpToSection(FName("ThrowSpell"));
					FTimerHandle SpawnSpellTimer;
					GetWorldTimerManager().SetTimer(SpawnSpellTimer, this, &ANightShade::ThrowSpell, 1.f);
					FTimerHandle ResetSpellTimer;
					GetWorldTimerManager().SetTimer(ResetSpellTimer, this, &ANightShade::ResetCanSpell, 8.f);
				}
			}
		}
	}
}

void ANightShade::SpawnLichFunc()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(CombatState == ECombatState::ECS_Unoccupied && AnimInstance)
	{
		CombatState = ECombatState::ECS_FireTimerInProgress;
		AnimInstance->Montage_Play(SpawnLichMontage);
		AnimInstance->Montage_JumpToSection(FName("SpawnLich"));
		FTimerHandle SpawnLichTimer;
		GetWorldTimerManager().SetTimer(SpawnLichTimer, this, &ANightShade::SpawnLich, 0.75f);
	}
}

void ANightShade::CanSpawnLich()
{
	bCanSpawnLich = true;
	bSpawnedLich = false;
	FTimerHandle Trigger;
	GetWorldTimerManager().SetTimer(Trigger, this, &ANightShade::CanSpawnLich, 15.f);
}

void ANightShade::SpawnLich()
{
	if(LichToSpawn)
	{
		CombatState = ECombatState::ECS_Unoccupied;
		bSpawnedLich = true;
		for(int j = 0; j <= 5 ; j++)
		{
			const float RandomX = FMath::FRandRange(-1000.f,1000.f);
			const float RandomY = FMath::FRandRange(-1000.f,1000.f);
			const FVector LichSpawnLocation(GetActorLocation().X + RandomX, GetActorLocation().Y + RandomY, GetActorLocation().Z);
			GetWorld()->SpawnActor<ACharacter>(LichToSpawn, LichSpawnLocation , GetActorRotation());	
		}
	}
}

void ANightShade::ResetMovementSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void ANightShade::ThrowSpell()
{
	const USkeletalMeshSocket* SpellSpawnSocket = GetMesh()->GetSocketByName(FName("SpawnSpellSocket"));
	if(SpellSpawnSocket && SpellToSpawn)
	{
		const FTransform SocketTransform = SpellSpawnSocket->GetSocketTransform(GetMesh());
		FVector SpellSpawnLocation = SocketTransform.GetLocation();
		GetWorld()->SpawnActor<AActor>(SpellToSpawn, SpellSpawnLocation , GetActorRotation());
		if(i < 15)
		{
			FTimerHandle TriggerTimer;
			GetWorldTimerManager().SetTimer(TriggerTimer, this, &ANightShade::ThrowSpell, 0.1f);
			i++;
		}
		else
		{
			i = 0;
		}
	}
}

void ANightShade::CosmicSphereRef(AActor* CosmicSphere)
{
	CosmicSphereReference = CosmicSphere;
}

void ANightShade::ResetMovementSpeedTrigger()
{
	FTimerHandle ResetMovementSpeedTimer;
	GetWorldTimerManager().SetTimer(ResetMovementSpeedTimer, this, &ANightShade::ResetMovementSpeed, 5.f);
}

void ANightShade::ResetCanSpell()
{
	bCanSpell = true;
}

void ANightShade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateToCharacter();
	CalculateDistance();
	if(CosmicSphereReference && bCosmicSphereRange)
	{
		FVector NewLocation = GetActorLocation();
		NewLocation.Y = FMath::FInterpTo(GetActorLocation().Y, CosmicSphereReference->GetActorLocation().Y, DeltaTime, 1.f);
		NewLocation.X = FMath::FInterpTo(GetActorLocation().X, CosmicSphereReference->GetActorLocation().X, DeltaTime, 1.f);
		SetActorLocation(NewLocation);
	}
	if(bCanSpawnLich && CombatState == ECombatState::ECS_Unoccupied && !bSpawnedLich)
	{
		bCanSpawnLich = false;
		SpawnLichFunc();
	}
}
