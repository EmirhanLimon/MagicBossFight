
#include "NightShadeAnimInstance.h"

#include "NightShade.h"

void UNightShadeAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if(NightShade == nullptr)
	{
		NightShade = Cast<ANightShade>(TryGetPawnOwner());
	}
	if(NightShade)
	{
		FVector Velocity { NightShade->GetVelocity() };
		Velocity.Z = 0.f;
		Speed = Velocity.Size();
	}
}
