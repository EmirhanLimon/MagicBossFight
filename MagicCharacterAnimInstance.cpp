


#include "MagicCharacterAnimInstance.h"

#include "MagicCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMagicCharacterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if(Character == nullptr)
	{
		Character = Cast<AMagicCharacter>(TryGetPawnOwner());
	}
	if(Character)
	{
		FVector Velocity{ Character->GetVelocity() };
		Velocity.Z = 0;
		Speed = Velocity.Size();

		if(Character->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
		{
			bIsAccelerating = true;
		}
		else
		{
			bIsAccelerating = false;
		}
		FRotator AimRotation = Character->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(Character->GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;
	}
}

void UMagicCharacterAnimInstance::NativeInitializeAnimation()
{
	Character = Cast<AMagicCharacter>(TryGetPawnOwner());
}
