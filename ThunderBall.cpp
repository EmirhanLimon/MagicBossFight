
#include "ThunderBall.h"

#include "Lich.h"
#include "NavigationSystemTypes.h"
#include "NightShade.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


AThunderBall::AThunderBall()
{
	PrimaryActorTick.bCanEverTick = true;

	ThunderBallSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ThunderBallSphereComponent"));
	ThunderBallSphereComponent->SetupAttachment(GetRootComponent());

	ThunderBallParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ThunderBallParticleSystemComponent"));
	ThunderBallParticleSystemComponent->SetupAttachment(ThunderBallSphereComponent);
	ThunderBallParticleSystemComponent->SetActive(true);
}

void AThunderBall::BeginPlay()
{
	Super::BeginPlay();
	ThunderBallSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AThunderBall::BeginOverlap);
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AThunderBall::Destroyed, 1.6f);
	ThunderBallSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AThunderBall::BeginOverlap);
}

void AThunderBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(GetActorLocation() + ((GetActorForwardVector() * 1250.f) * DeltaTime));
}

void AThunderBall::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ALich* Lich = Cast<ALich>(OtherActor);
	ANightShade* NightShade = Cast<ANightShade>(OtherActor);
	if(OtherActor == nullptr) return;
	if(Lich)
	{
		Lich->SetLichHealth(Lich->GetLichHealth() - 20.f);
		GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Blue,TEXT("123"));
	}
	if(NightShade)
	{
		NightShade->SetNightShadeHealth(NightShade->GetNightShadeHealth() - 20.f);
		GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Purple,TEXT("123"));
	}
}

void AThunderBall::Destroyed()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ThunderBallHitParticleSystem,GetActorLocation() ,GetActorRotation());
	Destroy();
}

