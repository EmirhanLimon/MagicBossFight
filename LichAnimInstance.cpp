
#include "LichAnimInstance.h"

#include "Lich.h"

void ULichAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if(Lich == nullptr)
	{
		Lich = Cast<ALich>(TryGetPawnOwner());
	}
	if(Lich)
	{
		FVector Velocity { Lich->GetVelocity() };
		Velocity.Z = 0.f;
		Speed = Velocity.Size();
	}
}
