#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterActionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterActionInterface : public UInterface
{
	GENERATED_BODY()
};

class CPP_PORTPOLIO_API ICharacterActionInterface
{
	GENERATED_BODY()

public:
#pragma region CharacterMovementInterface
	virtual void Walk() PURE_VIRTUAL(ICharacterActionInterface::Walk)
	virtual void Mover() PURE_VIRTUAL(ICharacterActionInterface::Mover)
	virtual void Crouch() PURE_VIRTUAL(ICharacterActionInterface::Crouch)

	virtual void StartSprint() PURE_VIRTUAL(ICharacterActionInterface::StartSprint)
	virtual void EndSprint() PURE_VIRTUAL(ICharacterActionInterface::EndSprint)
#pragma endregion

#pragma region CharacterActionInterface
	virtual void BaseAttack()	PURE_VIRTUAL(ICharacterActionInterface::BaseAttack)

	virtual void BeginActionA() PURE_VIRTUAL(ICharacterActionInterface::BeginActionA)
#pragma endregion

#pragma region CharacterInteraction
	virtual void Interaction() PURE_VIRTUAL(ICharacterActionInterface::Interaction)
#pragma endregion

#pragma region CharacterVaulting
	virtual void GrabLedge() PURE_VIRTUAL(ICharacterActionInterface::GrabLedge)
	virtual void ExitLedge() PURE_VIRTUAL(ICharacterActionInterface::ExitLedge)
	virtual void ClimbLedge() PURE_VIRTUAL(ICharacterActionInterface::ClimbLedge)

	virtual	void CanGrab(bool CanGrabBI) PURE_VIRTUAL(ICharacterActionInterface::CanGrab)
	virtual void SetClimbLedge(bool IsClimbing) PURE_VIRTUAL(ICharacterActionInterface::SetClimbLedge)

	virtual void ClimbWall() PURE_VIRTUAL(ICharacterActionInterface::ClimbWall)
#pragma endregion 

	virtual void TakeDown() PURE_VIRTUAL(ICharacterActionInterface::TakeDown)
};
