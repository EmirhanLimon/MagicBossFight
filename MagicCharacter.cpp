
#include "MagicCharacter.h"

#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "Lich.h"
#include "NightShade.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


AMagicCharacter::AMagicCharacter() :
BaseTurnRate(45.f),
BaseLookUpRate(45.f),
DashDistance(16000.f),
CharacterHealth(100.f),
bDashing(false),
bCanDash(true),
DashCoolDown(0.f),
bFireWallUsing(false),
LineTraceHitLocation(FVector(0,0,0)),
DarkShotCoolDown(0.f),
IceShotCoolDown(0.f),
MeteorCoolDown(0.f),
ThunderStormCoolDown(0.f),
ThunderBallCoolDown(0.f),
FireWallCoolDown(0.f),
CosmicSphereCoolDown(0.f),
RecoverCoolDown(0.f)
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetupAttachment(GetRootComponent());

	FireSphere1 = CreateDefaultSubobject<USphereComponent>(TEXT("FireSphere1"));
	FireSphere1->SetupAttachment(SceneComponent);
	FireSphere2 = CreateDefaultSubobject<USphereComponent>(TEXT("FireSphere2"));
	FireSphere2->SetupAttachment(SceneComponent);
	FireSphere3 = CreateDefaultSubobject<USphereComponent>(TEXT("FireSphere3"));
	FireSphere3->SetupAttachment(SceneComponent);
	FireSphere4 = CreateDefaultSubobject<USphereComponent>(TEXT("FireSphere4"));
	FireSphere4->SetupAttachment(SceneComponent);

	FireWallParticleSystemComponent1 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireWallParticleSystem1"));
	FireWallParticleSystemComponent1->SetupAttachment(FireSphere1);
	FireWallParticleSystemComponent2 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireWallParticleSystem2"));
	FireWallParticleSystemComponent2->SetupAttachment(FireSphere2);
	FireWallParticleSystemComponent3 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireWallParticleSystem3"));
	FireWallParticleSystemComponent3->SetupAttachment(FireSphere3);
	FireWallParticleSystemComponent4 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireWallParticleSystem4"));
	FireWallParticleSystemComponent4->SetupAttachment(FireSphere4);

	RecoverParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RecoverParticleSystemComponent"));
	RecoverParticleSystemComponent->SetupAttachment(GetRootComponent());

	PawnNoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitter"));
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
}

void AMagicCharacter::BeginPlay()
{
	Super::BeginPlay();
	FireSphere1->OnComponentBeginOverlap.AddDynamic(this, &AMagicCharacter::BeginOverlap);
	FireSphere2->OnComponentBeginOverlap.AddDynamic(this, &AMagicCharacter::BeginOverlap);
	FireSphere3->OnComponentBeginOverlap.AddDynamic(this, &AMagicCharacter::BeginOverlap);
	FireSphere4->OnComponentBeginOverlap.AddDynamic(this, &AMagicCharacter::BeginOverlap);
}

void AMagicCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && CombatState == ECombatState::ECS_Unoccupied) {
		
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };
		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };
		AddMovementInput(Direction, Value);
	}
}

void AMagicCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && CombatState == ECombatState::ECS_Unoccupied) {

		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };
		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };
		AddMovementInput(Direction, Value);
	}
}

void AMagicCharacter::TurnAtRate(float Rate)
{
	if(CombatState == ECombatState::ECS_Unoccupied)
	{
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AMagicCharacter::LookUpAtRate(float Rate)
{
	if(CombatState == ECombatState::ECS_Unoccupied)
	{
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void AMagicCharacter::Dashing()
{
	if(bCanDash && DashCoolDown == 0 && CombatState == ECombatState::ECS_Unoccupied)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		const FVector ForwardDir = this->GetActorRotation().Vector();
		LaunchCharacter(ForwardDir * DashDistance, true, true);
		if(AnimInstance)
		{
			AnimInstance->Montage_Play(DashMontage);
			AnimInstance->Montage_JumpToSection(FName("Dash"));
			bDashing = true;
			bCanDash = false;
			DashCoolDown = 5.f;
			DashTimerFunc();
		}	
	}
}

void AMagicCharacter::AddControllerYawInput(float Val)
{
	if(!bDashing && CombatState == ECombatState::ECS_Unoccupied)
	{
		Super::AddControllerYawInput(Val);
	}
}

void AMagicCharacter::AddControllerPitchInput(float Val)
{
	if(!bDashing && CombatState == ECombatState::ECS_Unoccupied)
	{
		Super::AddControllerPitchInput(Val);
	}
}

void AMagicCharacter::DashTimerFunc()
{
	if(DashCoolDown >= 1.f)
	{
		GetWorldTimerManager().SetTimer(DashTimer, this, &AMagicCharacter::DashTimerFunc, 1.f);
		DashCoolDown--;
	}
	else
	{
		DashCoolDown = 0.f;
		bCanDash = true;
	}
}

void AMagicCharacter::ThunderBallFunc()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && ThunderBallToSpawn && CombatState == ECombatState::ECS_Unoccupied && ThunderBallCoolDown == 0.f)
	{
		ThunderBallCoolDown = 10.f;
		ThunderBallCoolDownFunc();
		AnimInstance->Montage_Play(ThunderBallMontage);
		AnimInstance->Montage_JumpToSection(FName("ThunderBall"));
		for(int i = 0; i <= 100; i++)
		{
			CombatState = ECombatState::ECS_FireTimerInProgress;
			const float RandomLocationX = FMath::RandRange(-2000.f , 2000.f);
			const float RandomLocationY = FMath::RandRange(-2000.f , 2000.f);
			const FVector ThunderBallSpawnLocation{ GetActorLocation().X + RandomLocationX, GetActorLocation().Y + RandomLocationY, GetActorLocation().Z + 2000.f};
			GetWorld()->SpawnActor<AActor>(ThunderBallToSpawn, ThunderBallSpawnLocation, GetActorRotation());
		}
	}
}

void AMagicCharacter::DarkShotFunc()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && CombatState == ECombatState::ECS_Unoccupied && ShotMontage && DarkShotCoolDown == 0.f)
	{
		DarkShotCoolDown = 7.f;
		DarkShotCoolDownFunc();
		LineTrace();
		AnimInstance->Montage_Play(ShotMontage);
		AnimInstance->Montage_JumpToSection(FName("DarkShot"));
		CombatState = ECombatState::ECS_FireTimerInProgress;
		GetWorldTimerManager().SetTimer(DarkShotSpawnTimer, this, &AMagicCharacter::SpawnDarkShot,1.f);
	}
}

void AMagicCharacter::IceShotFunc()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && CombatState == ECombatState::ECS_Unoccupied && ShotMontage && IceShotCoolDown == 0.f)
	{
		IceShotCoolDown = 8.f;
		IceShotCoolDownFunc();
		LineTrace();
		AnimInstance->Montage_Play(ShotMontage);
		AnimInstance->Montage_JumpToSection(FName("IceShot"));
		CombatState = ECombatState::ECS_FireTimerInProgress;
		GetWorldTimerManager().SetTimer(IceShotSpawnTimer, this, &AMagicCharacter::SpawnIceShot,0.3f);
	}
}

void AMagicCharacter::SpawnDarkShot()
{
	const USkeletalMeshSocket* DarkShotSpawnSocket = GetMesh()->GetSocketByName(FName("ShotSpawnSocketRight"));
	if(DarkShotSpawnSocket && DarkShotToSpawn)
	{
		const FTransform SocketTransform = DarkShotSpawnSocket->GetSocketTransform(GetMesh());
		FVector DarkShotSpawnLocation = SocketTransform.GetLocation();
		GetWorld()->SpawnActor<AActor>(DarkShotToSpawn, DarkShotSpawnLocation, GetActorRotation());
	}
}

void AMagicCharacter::SpawnIceShot()
{
	const USkeletalMeshSocket* IceShotSpawnSocket = GetMesh()->GetSocketByName(FName("ShotSpawnSocketRight"));
	if(IceShotSpawnSocket && IceShotToSpawn)
	{
		const FTransform SocketTransform = IceShotSpawnSocket->GetSocketTransform(GetMesh());
		FVector IceShotSpawnLocation = SocketTransform.GetLocation();
		GetWorld()->SpawnActor<AActor>(IceShotToSpawn, IceShotSpawnLocation, GetActorRotation());
	}
}

void AMagicCharacter::LineTrace()
{
	FVector2D ViewportSize;
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	FHitResult HitResult;
	FVector2D CrossHairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrossHairWorldPosition;
	FVector CrossHairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
			CrossHairLocation,CrossHairWorldPosition, CrossHairWorldDirection);
	if(bScreenToWorld)
	{
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(LichReference);
		CollisionParams.AddIgnoredActor(NightShadeReference);
		const FVector Start { CrossHairWorldPosition };
		const FVector End { Start + CrossHairWorldDirection * 50'000.f };
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility,CollisionParams);
		if(HitResult.bBlockingHit)
		{
			GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Blue,FString::Printf(TEXT("%s"),*HitResult.GetActor()->GetActorNameOrLabel()));
			LineTraceHitLocation = HitResult.Location;
		}
	}
}

void AMagicCharacter::FireWall(float DeltaTime)
{
	FRotator Rotation(SceneComponent->GetComponentRotation().Pitch,SceneComponent->GetComponentRotation().Yaw + (1000.f *DeltaTime),SceneComponent->GetComponentRotation().Roll);
	SceneComponent->SetWorldRotation(Rotation);
}

void AMagicCharacter::FireWallFunc()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && CombatState == ECombatState::ECS_Unoccupied && FireWallMontage && !bFireWallUsing && FireWallCoolDown == 0.f)
	{
		FireWallCoolDown = 20.f;
		FireWallCoolDownFunc();
		AnimInstance->Montage_Play(FireWallMontage);
		AnimInstance->Montage_JumpToSection(FName("FireWall"));
		bFireWallUsing = true;
		FireWallParticleSystemComponent1->SetVisibility(true);
		FireWallParticleSystemComponent2->SetVisibility(true);
		FireWallParticleSystemComponent3->SetVisibility(true);
		FireWallParticleSystemComponent4->SetVisibility(true);
		FTimerHandle FireWallEndTimer;
		CombatState = ECombatState::ECS_FireTimerInProgress;
		GetWorldTimerManager().SetTimer(FireWallEndTimer, this, &AMagicCharacter::FireWallEndFunc, 10.f);
	}

}

void AMagicCharacter::FireWallEndFunc()
{
	bFireWallUsing = false;
	FireWallParticleSystemComponent1->SetVisibility(false);
	FireWallParticleSystemComponent2->SetVisibility(false);
	FireWallParticleSystemComponent3->SetVisibility(false);
	FireWallParticleSystemComponent4->SetVisibility(false);
}

void AMagicCharacter::MeteorFunc()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && CombatState == ECombatState::ECS_Unoccupied && MeteorMontage && MeteorCoolDown == 0.f)
	{
		MeteorCoolDown = 27.f;
		MeteorCoolDownFunc();
		AnimInstance->Montage_Play(MeteorMontage);
		AnimInstance->Montage_JumpToSection(FName("Meteor"));
		CombatState = ECombatState::ECS_FireTimerInProgress;
		FTimerHandle BlackHoleSpawnTimer;
		GetWorldTimerManager().SetTimer(BlackHoleSpawnTimer, this, &AMagicCharacter::MeteorSpawn, 3.f);
	}
}

void AMagicCharacter::MeteorSpawn()
{
	if(MeteorToSpawn)
	{
		const FVector MeteorSpawnLocation(GetActorLocation().X , GetActorLocation().Y, GetActorLocation().Z + 3500.f );
		GetWorld()->SpawnActor<AActor>(MeteorToSpawn, MeteorSpawnLocation + (GetActorForwardVector() * 750.f), GetActorRotation());
	}
}

void AMagicCharacter::ThunderStormFunc()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && CombatState == ECombatState::ECS_Unoccupied && ThunderStormMontage && ThunderStormCoolDown == 0.f)
	{
		ThunderStormCoolDown = 15.f;
		ThunderStormCoolDownFunc();
		AnimInstance->Montage_Play(ThunderStormMontage);
		AnimInstance->Montage_JumpToSection(FName("ThunderStorm"));
		CombatState = ECombatState::ECS_FireTimerInProgress;
		FTimerHandle ThunderStormSpawnTimer;
		GetWorldTimerManager().SetTimer(ThunderStormSpawnTimer, this, &AMagicCharacter::ThunderStormSpawn, 1.f);
	}
}

void AMagicCharacter::ThunderStormSpawn()
{
	if(ThunderStormToSpawn)
	{
		const FVector ThunderStormSpawnLocation(GetActorLocation().X , GetActorLocation().Y, GetActorLocation().Z +250.f );
		GetWorld()->SpawnActor<AActor>(ThunderStormToSpawn, ThunderStormSpawnLocation + (GetActorForwardVector() * 200.f), GetActorRotation());
	}
}

void AMagicCharacter::RecoverFunc()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(RecoverParticleSystemComponent && CombatState == ECombatState::ECS_Unoccupied && AnimInstance && RecoverCoolDown == 0.f)
	{
		RecoverCoolDown = 15.f;
		RecoverCoolDownFunc();
		AnimInstance->Montage_Play(RecoverMontage);
		AnimInstance->Montage_JumpToSection(FName("Recover"));
		CombatState = ECombatState::ECS_FireTimerInProgress;
		FTimerHandle RecoverStartTimer;
		GetWorldTimerManager().SetTimer(RecoverStartTimer, this, &AMagicCharacter::RecoverVisible,2.5f);
		FTimerHandle RecoverEndTimer;
		GetWorldTimerManager().SetTimer(RecoverEndTimer, this, &AMagicCharacter::RecoverEndFunc,10.f);
	}
}

void AMagicCharacter::RecoverHp()
{
	
	if(RecoverParticleSystemComponent->IsVisible())
	{
		FTimerHandle RecoverTriggerTimer;
		GetWorldTimerManager().SetTimer(RecoverTriggerTimer, this, &AMagicCharacter::RecoverHp,0.5f);
		if(CharacterHealth >= 95.f)
		{
			CharacterHealth = 100;
		}
		else
		{
			CharacterHealth = CharacterHealth + 3.f;
		}
	}
}

void AMagicCharacter::RecoverEndFunc()
{
	RecoverParticleSystemComponent->SetVisibility(false);
}

void AMagicCharacter::RecoverVisible()
{
	if(RecoverParticleSystemComponent)
	{
		RecoverParticleSystemComponent->SetVisibility(true);
		RecoverHp();
	}
}

void AMagicCharacter::CosmicSphereFunc()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && CosmicSphereCoolDown == 0.f && CombatState == ECombatState::ECS_Unoccupied && CosmicSphereMontage)
	{
		CosmicSphereCoolDown = 20.f;
		CosmicSphereCoolDownFunc();
		AnimInstance->Montage_Play(CosmicSphereMontage);
		AnimInstance->Montage_JumpToSection(FName("CosmicSphere"));
		CombatState = ECombatState::ECS_FireTimerInProgress;
		FTimerHandle CosmicSphereSpawnTimer;
		GetWorldTimerManager().SetTimer(CosmicSphereSpawnTimer, this, &AMagicCharacter::CosmicSphereSpawn, 1.f);
	}
}

void AMagicCharacter::CosmicSphereSpawn()
{
	if(CosmicSphereToSpawn)
	{
		const FVector CosmicSphereSpawnLocation(GetActorLocation().X , GetActorLocation().Y, GetActorLocation().Z + 125.f);
		GetWorld()->SpawnActor<AActor>(CosmicSphereToSpawn, CosmicSphereSpawnLocation + (GetActorForwardVector() * 750.f), GetActorRotation());
	}
}

void AMagicCharacter::DarkShotCoolDownFunc()
{
	if(DarkShotCoolDown >= 1)
	{
		GetWorldTimerManager().SetTimer(DarkShotCoolDownTimer, this, &AMagicCharacter::DarkShotCoolDownFunc, 1.f);
		DarkShotCoolDown--;
	}
	else
	{
		DarkShotCoolDown = 0;
	}
}

void AMagicCharacter::IceShotCoolDownFunc()
{
	if(IceShotCoolDown >= 1)
	{
		GetWorldTimerManager().SetTimer(IceShotCoolDownTimer, this, &AMagicCharacter::IceShotCoolDownFunc, 1.f);
		IceShotCoolDown--;
	}
	else
	{
		IceShotCoolDown = 0;
	}
}

void AMagicCharacter::MeteorCoolDownFunc()
{
	if(MeteorCoolDown >= 1)
	{
		GetWorldTimerManager().SetTimer(MeteorCoolDownTimer, this, &AMagicCharacter::MeteorCoolDownFunc, 1.f);
		MeteorCoolDown--;
	}
	else
	{
		MeteorCoolDown = 0;
	}
}

void AMagicCharacter::ThunderStormCoolDownFunc()
{
	if(ThunderStormCoolDown >= 1)
	{
		GetWorldTimerManager().SetTimer(ThunderStormCoolDownTimer, this, &AMagicCharacter::ThunderStormCoolDownFunc, 1.f);
		ThunderStormCoolDown--;
	}
	else
	{
		ThunderStormCoolDown = 0;
	}
}

void AMagicCharacter::ThunderBallCoolDownFunc()
{
	if(ThunderBallCoolDown >= 1)
	{
		GetWorldTimerManager().SetTimer(ThunderBallCoolDownTimer, this, &AMagicCharacter::ThunderBallCoolDownFunc, 1.f);
		ThunderBallCoolDown--;
	}
	else
	{
		ThunderBallCoolDown = 0;
	}
}

void AMagicCharacter::FireWallCoolDownFunc()
{
	if(FireWallCoolDown >= 1)
	{
		GetWorldTimerManager().SetTimer(FireWallCoolDownTimer, this, &AMagicCharacter::FireWallCoolDownFunc, 1.f);
		FireWallCoolDown--;
	}
	else
	{
		FireWallCoolDown = 0;
	}
}

void AMagicCharacter::CosmicSphereCoolDownFunc()
{
	if(CosmicSphereCoolDown >= 1)
	{
		GetWorldTimerManager().SetTimer(CosmicSphereCoolDownTimer, this, &AMagicCharacter::CosmicSphereCoolDownFunc, 1.f);
		CosmicSphereCoolDown--;
	}
	else
	{
		CosmicSphereCoolDown = 0;
	}
}

void AMagicCharacter::RecoverCoolDownFunc()
{
	if(RecoverCoolDown >= 1)
	{
		GetWorldTimerManager().SetTimer(RecoverCoolDownTimer, this, &AMagicCharacter::RecoverCoolDownFunc, 1.f);
		RecoverCoolDown--;
	}
	else
	{
		RecoverCoolDown = 0;
	}
}

void AMagicCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ALich* Lich = Cast<ALich>(OtherActor);
	ANightShade* NightShade = Cast<ANightShade>(OtherActor);
	if(OtherActor == nullptr) return;
	if(bFireWallUsing)
	{
		if(Lich)
		{
			Lich->SetLichHealth(Lich->GetLichHealth() - 1.5f);
		}
		if(NightShade)
		{
			NightShade->SetNightShadeHealth(NightShade->GetNightShadeHealth() - 1.5f);
		}
	}

}

void AMagicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bFireWallUsing)
	{
		FireWall(DeltaTime);
	}
	PawnNoiseEmitterComponent->MakeNoise(this,1.0f,GetActorLocation());
}

void AMagicCharacter::LichRef(ACharacter* LichRef)
{
	LichReference = LichRef;
}

void AMagicCharacter::NightShadeRef(ACharacter* NightShadeRef)
{
	NightShadeReference = NightShadeRef;
}

void AMagicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMagicCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMagicCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMagicCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMagicCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("Turn", this, &AMagicCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMagicCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMagicCharacter::Dashing);
	PlayerInputComponent->BindAction("ThunderBall", IE_Pressed, this, &AMagicCharacter::ThunderBallFunc);
	PlayerInputComponent->BindAction("DarkShot", IE_Pressed, this, &AMagicCharacter::DarkShotFunc);
	PlayerInputComponent->BindAction("IceShot", IE_Pressed, this, &AMagicCharacter::IceShotFunc);
	PlayerInputComponent->BindAction("FireWall", IE_Pressed, this, &AMagicCharacter::FireWallFunc);
	PlayerInputComponent->BindAction("Meteor", IE_Pressed, this, &AMagicCharacter::MeteorFunc);
	PlayerInputComponent->BindAction("ThunderStorm", IE_Pressed, this, &AMagicCharacter::ThunderStormFunc);
	PlayerInputComponent->BindAction("Recover", IE_Pressed, this, &AMagicCharacter::RecoverFunc);
	PlayerInputComponent->BindAction("CosmicSphere", IE_Pressed, this, &AMagicCharacter::CosmicSphereFunc);
}




