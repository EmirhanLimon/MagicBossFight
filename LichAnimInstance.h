

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LichAnimInstance.generated.h"


UCLASS()
class BOSSFIGHT_API ULichAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class ALich* Lich;
};
