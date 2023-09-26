
#include "ThunderStorm.h"

#include "Lich.h"
#include "NightShade.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AThunderStorm::AThunderStorm()
{
	PrimaryActorTick.bCanEverTick = true;

	ThunderStormCollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ThunderStormCollisionCapsule"));
	ThunderStormCollisionCapsule->SetupAttachment(GetRootComponent());

	ThunderStormParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ThunderStormParticle"));
	ThunderStormParticleSystemComponent->SetupAttachment(ThunderStormCollisionCapsule);
	ThunderStormParticleSystemComponent->Activate(true);
}

void AThunderStorm::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<AMagicCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AThunderStorm::DestroyFunction, 7.5f);
	ThunderStormCollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AThunderStorm::BeginOverlap);
}

void AThunderStorm::DestroyFunction()
{
	Destroy();
}

void AThunderStorm::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ALich* Lich = Cast<ALich>(OtherActor);
	ANightShade* NightShade = Cast<ANightShade>(OtherActor);
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

void AThunderStorm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(GetActorLocation() + ((GetActorForwardVector() * 300.f) * DeltaTime));
}

