


#include "CosmicSphere.h"

#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"


ACosmicSphere::ACosmicSphere()
{
	PrimaryActorTick.bCanEverTick = true;

	CosmicSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CosmicSphere"));
	CosmicSphere->SetupAttachment(GetRootComponent());

	CosmicSphereParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CosmisSphereParticleSystemComponent"));
	CosmicSphereParticleSystemComponent->SetupAttachment(CosmicSphere);

	CombatRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatRangeSphere"));
	CombatRangeSphere->SetupAttachment(CosmicSphere);
}


void ACosmicSphere::BeginPlay()
{
	Super::BeginPlay();
	CombatRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ACosmicSphere::CombatRangeOverlap);
	CombatRangeSphere->OnComponentEndOverlap.AddDynamic(this, &ACosmicSphere::CombatRangeOverlapEnd);
	CosmicSphere->OnComponentBeginOverlap.AddDynamic(this, &ACosmicSphere::BeginOverlap);
	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &ACosmicSphere::Destroyed,5.f);
}

void ACosmicSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACosmicSphere::CombatRangeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Lich = Cast<ALich>(OtherActor);
	NightShade = Cast<ANightShade>(OtherActor);
	if(NightShade)
	{
		NightShade->SetCosmisSphereRange(true);
		NightShade->CosmicSphereRef(this);
	}
	if(Lich)
	{
		Lich->SetCosmisSphereRange(true);
		Lich->CosmicSphereRef(this);
	}
}

void ACosmicSphere::CombatRangeOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Lich = Cast<ALich>(OtherActor);
	NightShade = Cast<ANightShade>(OtherActor);
	if(NightShade)
	{
		NightShade->SetCosmisSphereRange(false);
	}
	if(Lich)
	{
		Lich->SetCosmisSphereRange(false);
	}
}

void ACosmicSphere::Destroyed()
{
	Destroy();
}

void ACosmicSphere::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Lich = Cast<ALich>(OtherActor);
	NightShade = Cast<ANightShade>(OtherActor);
	if(OtherActor == nullptr) return;
	if(Lich)
	{
		Lich->SetLichHealth(Lich->GetLichHealth() - 25.f);
	}
	if(NightShade)
	{
		NightShade->SetNightShadeHealth(NightShade->GetNightShadeHealth() - 25.f);
	}
}


