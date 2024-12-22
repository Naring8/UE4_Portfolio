#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterStateInterface.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	IDLE,
	ACTING,
	DAMAGED,
	DIED,
	MAX
};

UENUM(BlueprintType)
enum class ECharacterMovementState : uint8
{
	IDLE,
	CROUCH,
	WALK,
	RUN,
	JUMP,
	HANG,
	MAX
};

UINTERFACE(MinimalAPI)
class UCharacterStateInterface : public UInterface
{
	GENERATED_BODY()
};

class CPP_PORTPOLIO_API ICharacterStateInterface
{
	GENERATED_BODY()

public:
	virtual void InitCharacterState() PURE_VIRTUAL(ICharacterStateInterface::InitCharacterState)

	virtual void SetCharacterState(ECharacterState State) PURE_VIRTUAL(ICharacterStateInterface::SetCharacterState)
	virtual ECharacterState GetCharacterState() PURE_VIRTUAL(ICharacterStateInterface::GetCharacterState, return ECharacterState::IDLE;)

	virtual void SetCharacterMovementState(ECharacterMovementState MovementState) PURE_VIRTUAL(ECharacterMovementState::SetCharacterState)
	virtual ECharacterMovementState GetCharacterMovementState() PURE_VIRTUAL(ECharacterMovementState::GetCharacterMovementState, return ECharacterMovementState::IDLE;)
};
