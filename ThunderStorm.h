
#pragma once

#include "CoreMinimal.h"
#include "MagicCharacter.h"
#include "GameFramework/Actor.h"
#include "ThunderStorm.generated.h"

UCLASS()
class BOSSFIGHT_API AThunderStorm : public AActor
{
	GENERATED_BODY()
	
public:	
	AThunderStorm();

protected:
	virtual void BeginPlay() override;
	void DestroyFunction();
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* ThunderStormCollisionCapsule;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* ThunderStormParticleSystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess))
	AMagicCharacter* Character;
};
