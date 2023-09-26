
#include "Lich.h"
#include "AIController.h"
#include "DarkShot.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ALich::ALich() :
bCombatRange(false),
Distance(0.f),
bCosmicSphereRange(false),
LichHealth(100.f),
bCanAttack(true),
bCanLeftAttack(false),
bCanRightAttack(false),
bDie(false)
{
	PrimaryActorTick.bCanEverTick = true;

	LichPawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("LichPawnSensing"));

	IceShotNiagaraEffectComponent = CreateDefaultSubobject<USceneComponent>(TEXT("NiagaraEffectComponent"));
	IceShotNiagaraEffectComponent->SetupAttachment(GetRootComponent());

	LichLeftBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftBoxComponent"));
	LichLeftBoxComponent->SetupAttachment(GetMesh(), FName("weapon_l"));
	LichRightBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RightBoxComponent"));
	LichRightBoxComponent->SetupAttachment(GetMesh(), FName("weapon_r"));
}

void ALich::BeginPlay()
{
	Super::BeginPlay();
	AIC_Ref = Cast<AEnemyController>(GetController());
	Character = Cast<AMagicCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	LichPawnSensing->OnHearNoise.AddDynamic(this, &ALich::OnHearNoise);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCharacterMovement()->MaxWalkSpeed = 600;
	PlaySpawnMontage();
	if(Character)
	{
		Character->LichRef(this);
	}
	LichLeftBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALich::BeginOverlap);
	LichRightBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALich::BeginOverlap);
}

void ALich::OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume)
{
	if(OtherActor == nullptr) return;
	Character = Cast<AMagicCharacter>(OtherActor);
	if(Character && !bCombatRange && CombatState == ECombatState::ECS_Unoccupied && !bDie)
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

void ALich::CalculateDistance()
{
	if(Character && !bDie)
	{
		Distance = FVector::Distance(GetActorLocation(),Character->GetActorLocation());
		if(Distance >= 100.f)
		{
			bCombatRange = false;
		}
		else
		{
			bCombatRange = true;
			StopMovement();
			if(bCanAttack && CombatState == ECombatState::ECS_Unoccupied)
			{
				UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				if(AnimInstance && AttackMontage)
				{
					FTimerHandle ResetAttackTimer;
					GetWorldTimerManager().SetTimer(ResetAttackTimer, this, &ALich::ResetAttack, 3.f);
					bCanAttack = false;
					CombatState = ECombatState::ECS_FireTimerInProgress;
					const float RandomAnim = FMath::FRandRange(0.f,1.f);
					if(RandomAnim >= 0.5f)
					{
						AnimInstance->Montage_Play(AttackMontage);
						AnimInstance->Montage_JumpToSection(FName("LeftAttack"));
					}
					else
					{
						AnimInstance->Montage_Play(AttackMontage);
						AnimInstance->Montage_JumpToSection(FName("RightAttack"));
					}
				}
			}
		}
	}
}

void ALich::PlaySpawnMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && !bDie)
	{
		CombatState = ECombatState::ECS_FireTimerInProgress;
		StopMovement();
		AnimInstance->Montage_Play(SpawnMontage);
		AnimInstance->Montage_JumpToSection(FName("Spawn"));
	}
}

void ALich::StopMovement()
{
	if(!IsValid(AIC_Ref))
	{
		AIC_Ref = Cast<AEnemyController>(GetController());
	}
	else
	{
		AIC_Ref->StopMovement();
	}
}

void ALich::ResetMovementSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void ALich::ResetAttack()
{
	bCanAttack = true;
}

void ALich::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && DieMontage)
	{
		AnimInstance->Montage_Play(DieMontage);
		AnimInstance->Montage_JumpToSection(FName("Die"));
		FTimerHandle DestroyTimer;
		GetWorldTimerManager().SetTimer(DestroyTimer, this, &ALich::Destroyed,2.3f);
	}

}

void ALich::Destroyed()
{
	Destroy();
}

void ALich::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                         int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<AMagicCharacter>(OtherActor);
	if(OtherActor == nullptr) return;
	if(Character && (bCanLeftAttack || bCanRightAttack) && !bDie)
	{
		Character->SetCharacterHealth(Character->GetCharacterHealth() - 2.f);
		bCanLeftAttack = false;
		bCanRightAttack = false;
	}
}

void ALich::ResetMovementSpeedTrigger()
{
	FTimerHandle ResetMovementSpeedTimer;
	GetWorldTimerManager().SetTimer(ResetMovementSpeedTimer, this, &ALich::ResetMovementSpeed, 5.f);
}

void ALich::CosmicSphereRef(AActor* CosmicSphere)
{
	CosmicSphereReference = CosmicSphere;
}

void ALich::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CalculateDistance();
	if(CosmicSphereReference && bCosmicSphereRange && !bDie)
	{
		FVector NewLocation = GetActorLocation();
		NewLocation.Y = FMath::FInterpTo(GetActorLocation().Y, CosmicSphereReference->GetActorLocation().Y, DeltaTime, 1.f);
		NewLocation.X = FMath::FInterpTo(GetActorLocation().X, CosmicSphereReference->GetActorLocation().X, DeltaTime, 1.f);
		SetActorLocation(NewLocation);
	}
	if(LichHealth <= 0 && !bDie)
	{
		bDie = true;
		Die();
	}
}