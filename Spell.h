

#pragma once

#include "CoreMinimal.h"
#include "MagicCharacter.h"
#include "GameFramework/Actor.h"
#include "Spell.generated.h"

UCLASS()
class BOSSFIGHT_API ASpell : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpell();

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly,  meta = (AllowPrivateAccess))
	class USphereComponent* SpellCollision;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,  meta = (AllowPrivateAccess))
	class UParticleSystemComponent* SpellParticleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess))
	FVector TargetLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess))
	FVector MidPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess))
	FVector CurvedPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess))
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess))
	float MinCurveRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess))
	float MaxCurveRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess))
	float MinCurvePointDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess))
	float MaxCurvePointDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess))
	float MinAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess))
	float MaxAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess))
	bool bMidPointCalculatorControl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess))
	bool bCurveControlPointControl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess))
	AMagicCharacter* Character;
};
