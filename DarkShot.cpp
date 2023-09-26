
#include "DarkShot.h"

#include "Lich.h"
#include "NiagaraComponent.h"
#include "NightShade.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

ADarkShot::ADarkShot() :
ClampValue(0.f),
Alpha(0.f)
{
	PrimaryActorTick.bCanEverTick = true;

	DarkShotSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("DarkShotSphereComponent"));
	DarkShotSphereComponent->SetupAttachment(GetRootComponent());

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(DarkShotSphereComponent);
}


void ADarkShot::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<AMagicCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer,this,&ADarkShot::Destroyed,1.f);
	DarkShotSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ADarkShot::BeginOverlap);
}

void ADarkShot::Destroyed()
{
	Destroy();
}

void ADarkShot::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ALich* Lich = Cast<ALich>(OtherActor);
	ANightShade* NightShade = Cast<ANightShade>(OtherActor);
	if(OtherActor == nullptr) return;
	if(OtherActor == Lich)
	{
		Lich->SetLichHealth(Lich->GetLichHealth() - 20.f);
	}
	if(OtherActor == NightShade)
	{
		NightShade->SetNightShadeHealth(NightShade->GetNightShadeHealth() - 20.f);
	}
}

void ADarkShot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ClampValue =  FMath::Clamp((Alpha + 0.4f),0,1);
	Alpha = ClampValue;
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),Character->GetLineTraceHitLocation());
	FRotator NewRotation = FRotator(FQuat::Slerp(UE::Math::TQuat<double>(GetActorRotation()), UE::Math::TQuat<double>(TargetRotation), Alpha));
	SetActorRotation(NewRotation);
	SetActorLocation(GetActorLocation() + (GetActorForwardVector() * 20.f));
}

