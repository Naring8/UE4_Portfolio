#pragma once

#include "CoreMinimal.h"
#include <GameFramework/PlayerController.h>
#include "CharacterActionInterface.h"
#include "CP_PlayerController.generated.h"

UCLASS()
class CPP_PORTPOLIO_API ACP_PlayerController : public APlayerController, public ICharacterActionInterface
{
	GENERATED_BODY()

public:
	void SetupInputComponent() override;

private:
	void MoveFB(float const Value);
	void MoveLR(float const Value);
	void LookLR(float const Value);
	void LookUD(float const Value);

	void Walk() override;
	void Mover() override;
	void Crouch() override;

	void StartSprint() override;
	void EndSprint() override;

	void BaseAttack() override;

	void BeginActionA() override;

	void Interaction() override;

private:
#pragma region Character Vaulting
#pragma endregion
};
