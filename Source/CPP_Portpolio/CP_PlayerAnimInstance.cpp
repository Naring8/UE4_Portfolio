#include "CP_PlayerAnimInstance.h"
#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>

#include "CharacterStateInterface.h"

void UCP_PlayerAnimInstance::NativeUpdateAnimation(float const DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (auto* const PawnOwner = TryGetPawnOwner())
	{
		FVector const Velocity = PawnOwner->GetVelocity();

		Direction = CalculateDirection(Velocity, PawnOwner->GetActorRotation());
		Speed = Velocity.Size2D();

		Pitch = PawnOwner->GetBaseAimRotation().Pitch;	// Pawn에 들어있는 내용
		Yaw = PawnOwner->GetBaseAimRotation().Yaw;

		auto* const Character = Cast<ACharacter>(PawnOwner);
		auto* const CharacterState = Cast<ICharacterStateInterface>(Character);

		if (!Character->GetMovementComponent()->IsFalling())
			bIsJumped = false;
		else
			bIsJumped = true;
	}
}

void UCP_PlayerAnimInstance::Crouch()
{
	bIsCrouch = !bIsCrouch;
}

void UCP_PlayerAnimInstance::SetClimbLedge(bool IsClimbing)
{
	bIsClimbing = IsClimbing;

	if (IsClimbing)
	{
		Montage_Play(ClimbLedgeMontage);
		bIsHanging = false;
	}
}

void UCP_PlayerAnimInstance::ClimbWall()
{
	Montage_Play(ClimbWallMontage);
}
