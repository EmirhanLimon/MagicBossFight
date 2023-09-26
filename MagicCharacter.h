
#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "MagicCharacter.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_FireTimerInProgress UMETA(DisplayName = "FireTimerInProgress")
};

UCLASS()
class BOSSFIGHT_API AMagicCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMagicCharacter();

protected:
	virtual void BeginPlay() override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void Dashing();
	virtual void AddControllerYawInput(float Val) override;
	virtual void AddControllerPitchInput(float Val) override;
	void DashTimerFunc();
	void ThunderBallFunc();
	void DarkShotFunc();
	void IceShotFunc();
	void SpawnDarkShot();
	void SpawnIceShot();
	void LineTrace();
	void FireWall(float DeltaTime);
	void FireWallFunc();
	void FireWallEndFunc();
	void MeteorFunc();
	void MeteorSpawn();
	void ThunderStormFunc();
	void ThunderStormSpawn();
	void RecoverFunc();
	void RecoverHp();
	void RecoverEndFunc();
	void RecoverVisible();
	void CosmicSphereFunc();
	void CosmicSphereSpawn();
	void DarkShotCoolDownFunc();
	void IceShotCoolDownFunc();
	void MeteorCoolDownFunc();
	void ThunderStormCoolDownFunc();
	void ThunderBallCoolDownFunc();
	void FireWallCoolDownFunc();
	void CosmicSphereCoolDownFunc();
	void RecoverCoolDownFunc();
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void LichRef(ACharacter* LichRef);
	void NightShadeRef(ACharacter* NightShadeRef);
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* SceneComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* FireSphere1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* FireSphere2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* FireSphere3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* FireSphere4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* FireWallParticleSystemComponent1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* FireWallParticleSystemComponent2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* FireWallParticleSystemComponent3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* FireWallParticleSystemComponent4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* RecoverParticleSystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float DashDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DashMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ThunderBallMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ShotMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FireWallMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* MeteorMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ThunderStormMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* RecoverMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* CosmicSphereMontage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float CharacterHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	bool bDashing;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	bool bCanDash;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	bool bFireWallUsing;
	FTimerHandle DashTimer;
	FTimerHandle DarkShotSpawnTimer;
	FTimerHandle IceShotSpawnTimer;
	FTimerHandle DarkShotCoolDownTimer;
	FTimerHandle IceShotCoolDownTimer;
	FTimerHandle MeteorCoolDownTimer;
	FTimerHandle ThunderStormCoolDownTimer;
	FTimerHandle ThunderBallCoolDownTimer;
	FTimerHandle FireWallCoolDownTimer;
	FTimerHandle CosmicSphereCoolDownTimer;
	FTimerHandle RecoverCoolDownTimer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float DashCoolDown;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TSubclassOf<AActor> ThunderBallToSpawn;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TSubclassOf<AActor> MeteorToSpawn;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TSubclassOf<AActor> DarkShotToSpawn;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TSubclassOf<AActor> IceShotToSpawn;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TSubclassOf<AActor> ThunderStormToSpawn;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TSubclassOf<AActor> CosmicSphereToSpawn;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector LineTraceHitLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float DarkShotCoolDown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float IceShotCoolDown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float MeteorCoolDown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float ThunderStormCoolDown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float ThunderBallCoolDown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float FireWallCoolDown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float CosmicSphereCoolDown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float RecoverCoolDown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UPawnNoiseEmitterComponent* PawnNoiseEmitterComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	ACharacter* LichReference;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class ACharacter* NightShadeReference;
	TArray<AActor*> deneme;
public:
	FORCEINLINE FVector GetLineTraceHitLocation() const { return LineTraceHitLocation; }
	FORCEINLINE float GetCharacterHealth() const { return CharacterHealth; }
	FORCEINLINE void SetCharacterHealth(float NewCharacterHealth) { CharacterHealth = NewCharacterHealth; }
};
