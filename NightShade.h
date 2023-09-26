
#pragma once

#include "CoreMinimal.h"
#include "EnemyController.h"
#include "MagicCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/Character.h"
#include "NightShade.generated.h"


class AAIController;
class UPawnSensingComponent;

UENUM(BlueprintType)
enum class ENightShadeCombatState : uint8
{
	ENSCS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ENSCS_FireTimerInProgress UMETA(DisplayName = "FireTimerInProgress")
};

UCLASS()
class BOSSFIGHT_API ANightShade : public ACharacter
{
	GENERATED_BODY()

public:
	ANightShade();

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume);
	void RotateToCharacter();
	void CalculateDistance();
	void SpawnLichFunc();
	void CanSpawnLich();
	void SpawnLich();
	void ResetMovementSpeed();
	void ThrowSpell();
public:	
	virtual void Tick(float DeltaTime) override;
	void CosmicSphereRef(AActor* CosmicSphere);
	void ResetMovementSpeedTrigger();
	void ResetCanSpell();
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess))
	AMagicCharacter* Character;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess))
	AActor* CosmicSphereReference;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess))
	AEnemyController* AIC_Ref;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess))
	UPawnSensingComponent* NightShadePawnSensing;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess))
	bool bCombatRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ClampValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Alpha;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Distance;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bCosmicSphereRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float NightShadeHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* SpawnLichMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* SpellMontage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bCanSpawnLich;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bSpawnedLich;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TSubclassOf<ACharacter> LichToSpawn;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TSubclassOf<AActor> SpellToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	USceneComponent* IceShotNiagaraEffectComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	uint8 i;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bCanSpell;
	
public:
	FORCEINLINE bool GetCosmicSphereRange() const { return bCosmicSphereRange; }
	FORCEINLINE void SetCosmisSphereRange(bool NewCosmicSphereRange) { bCosmicSphereRange = NewCosmicSphereRange; }
	FORCEINLINE float GetNightShadeHealth() const { return NightShadeHealth; }
	FORCEINLINE void SetNightShadeHealth(float NewNightShadeHealth) { NightShadeHealth = NewNightShadeHealth; }
	FORCEINLINE USceneComponent* GetIceShotNiagaraEffectComponent() const { return IceShotNiagaraEffectComponent;}
};
