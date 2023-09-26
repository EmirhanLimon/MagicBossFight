
#include "IceShot.h"
#include "DarkShot.h"
#include "Lich.h"
#include "NightShade.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AIceShot::AIceShot() :
Alpha(0.f),
ClampValue(0.f)
{
	PrimaryActorTick.bCanEverTick = true;

	IceShotSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("DarkShotSphereComponent"));
	IceShotSphereComponent->SetupAttachment(GetRootComponent());
}

void AIceShot::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<AMagicCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(IceShotSphereComponent)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(IceShotNiagara,IceShotSphereComponent,NAME_None,FVector(0.f),FRotator(0.f),EAttachLocation::Type::KeepRelativeOffset,true);
	}
	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer,this,&AIceShot::Destroyed,1.f);
	IceShotSphereComponent->OnComponentBeginOverlap.AddDynamic(this,&AIceShot::BeginOverlap);
}

void AIceShot::Destroyed()
{
	Destroy();
}

void AIceShot::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ALich* Lich = Cast<ALich>(OtherActor);
	ANightShade* NightShade = Cast<ANightShade>(OtherActor);
	if(OtherActor == nullptr) return;
	if(OtherActor == Lich)
	{
		Lich->SetLichHealth(Lich->GetLichHealth() - 10.f);
		Lich->GetCharacterMovement()->MaxWalkSpeed = 100;
		Lich->ResetMovementSpeedTrigger();
		if(Lich->GetIceShotNiagaraEffectComponent())
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(IceShotHitNiagara,Lich->GetIceShotNiagaraEffectComponent(),NAME_None,FVector(0.f),FRotator(0.f),EAttachLocation::Type::KeepRelativeOffset,true);
		}
	}
	if(OtherActor == NightShade)
	{
		NightShade->SetNightShadeHealth(NightShade->GetNightShadeHealth() - 10.f);
		NightShade->GetCharacterMovement()->MaxWalkSpeed = 100;
		NightShade->ResetMovementSpeedTrigger();
		if(NightShade->GetIceShotNiagaraEffectComponent())
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(IceShotHitNiagara,NightShade->GetIceShotNiagaraEffectComponent(),NAME_None,FVector(0.f),FRotator(0.f),EAttachLocation::Type::KeepRelativeOffset,true);
		}
	}
}

void AIceShot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ClampValue =  FMath::Clamp((Alpha + 0.4f),0,1);
	Alpha = ClampValue;
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),Character->GetLineTraceHitLocation());
	FRotator NewRotation = FRotator(FQuat::Slerp(UE::Math::TQuat<double>(GetActorRotation()), UE::Math::TQuat<double>(TargetRotation), Alpha));
	SetActorRotation(NewRotation);
	SetActorLocation(GetActorLocation() + (GetActorForwardVector() * 20.f));
}

