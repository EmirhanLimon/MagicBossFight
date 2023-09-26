
#pragma once

#include "CoreMinimal.h"
#include "EnemyController.h"
#include "MagicCharacter.h"
#include "Components/BoxComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/Character.h"
#include "Lich.generated.h"

class AAIController;
class UPawnSensingComponent;

UENUM(BlueprintType)
enum class ELichCombatState : uint8
{
	ELCS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ELCS_FireTimerInProgress UMETA(DisplayName = "FireTimerInProgress")
};

UCLASS()
class BOSSFIGHT_API ALich : public ACharacter
{
	GENERATED_BODY()

public:
	ALich();

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume);
	void CalculateDistance();
	void PlaySpawnMontage();
	void StopMovement();
	void ResetMovementSpeed();
	void ResetAttack();
	void Die();
	virtual void Destroyed() override;
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	
	virtual void Tick(float DeltaTime) override;
	void CosmicSphereRef(AActor* CosmicSphere);
	void ResetMovementSpeedTrigger();
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess))
	AMagicCharacter* Character;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess))
	AActor* CosmicSphereReference;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess))
	AEnemyController* AIC_Ref;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess))
	UPawnSensingComponent* LichPawnSensing;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess))
	bool bCombatRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Distance;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bCosmicSphereRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float LichHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* SpawnMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DieMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	USceneComponent* IceShotNiagaraEffectComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* LichLeftBoxComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* LichRightBoxComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bCanAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bCanLeftAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bCanRightAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bDie;
public:
	FORCEINLINE bool GetCosmicSphereRange() const { return bCosmicSphereRange; }
	FORCEINLINE void SetCosmisSphereRange(bool NewCosmicSphereRange) { bCosmicSphereRange = NewCosmicSphereRange; }
	FORCEINLINE float GetLichHealth() const { return LichHealth; }
	FORCEINLINE void SetLichHealth(float NewLichHealth) { LichHealth = NewLichHealth; }
	FORCEINLINE USceneComponent* GetIceShotNiagaraEffectComponent() const { return IceShotNiagaraEffectComponent;}
};
