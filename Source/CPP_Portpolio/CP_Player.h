#pragma once

#include "CoreMinimal.h"
#include "CP_Character.h"
#include "CP_Player.generated.h"

UCLASS(Abstract)
class CPP_PORTPOLIO_API ACP_Player : public ACP_Character
{
	GENERATED_BODY()

public:
	ACP_Player();

private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

};
