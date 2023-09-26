
#include "Meteor.h"

#include "Lich.h"
#include "NightShade.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"


AMeteor::AMeteor()
{
	PrimaryActorTick.bCanEverTick = true;

	MeteorSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("MeteorSphereComponent"));
	MeteorSphereComponent->SetupAttachment(GetRootComponent());

	MeteorParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MeteorParticleSystemComponent"));
	MeteorParticleSystemComponent->SetupAttachment(MeteorSphereComponent);
	MeteorParticleSystemComponent->SetActive(true);

	MeteorHitComponent = CreateDefaultSubobject<USphereComponent>(TEXT("HitComponent"));
	MeteorHitComponent->SetupAttachment(MeteorParticleSystemComponent);
}


void AMeteor::BeginPlay()
{
	Super::BeginPlay();
	MeteorHitComponent->OnComponentBeginOverlap.AddDynamic(this, &AMeteor::AMeteor::BeginOverlap);
	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AMeteor::Destroyed, 2.5f);
}

void AMeteor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ALich* Lich = Cast<ALich>(OtherActor);
	ANightShade* NightShade = Cast<ANightShade>(OtherActor);
	if(OtherActor == nullptr) return;
	if(Lich)
	{
		Lich->SetLichHealth(Lich->GetLichHealth() - 25.f);
		GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Red,TEXT("Lich"));
	}
	if(NightShade)
	{
		NightShade->SetNightShadeHealth(NightShade->GetNightShadeHealth() - 25.f);
		GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Red,TEXT("NightShade"));
	}
}

void AMeteor::Destroyed()
{
	Destroy();
}

void AMeteor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(GetActorLocation() + ((GetActorForwardVector() * 2000.f) * DeltaTime));
}

