

#include "Spell.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

ASpell::ASpell() :
Speed(2000.f),
MinCurveRadius(40.f),
MaxCurveRadius(400.f),
MinCurvePointDistance(0.2f),
MaxCurvePointDistance(0.8f),
MinAngle(0.f),
MaxAngle(360.f),
TargetLocation(FVector(0,0,0)),
MidPoint(FVector(0,0,0)),
CurvedPoint(FVector(0,0,0))
{
	PrimaryActorTick.bCanEverTick = true;

	SpellCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SpellCollision"));
	SpellCollision->SetupAttachment(GetRootComponent());

	SpellParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SpellParticle"));
	SpellParticleComponent->SetupAttachment(SpellCollision);
}

void ASpell::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<AMagicCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	SpellCollision->OnComponentBeginOverlap.AddDynamic(this, &ASpell::BeginOverlap);
}

void ASpell::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<AMagicCharacter>(OtherActor);
	if(OtherActor == nullptr) return;
	if(Character)
	{
		Character->SetCharacterHealth(Character->GetCharacterHealth() - 0.75f);
		Destroy();
	}
}

void ASpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Character)
	{
		TargetLocation = Character->GetActorLocation();
	}
}

