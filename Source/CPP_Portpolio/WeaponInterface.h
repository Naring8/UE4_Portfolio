#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

UINTERFACE(MinimalAPI)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class CPP_PORTPOLIO_API IWeaponInterface
{
	GENERATED_BODY()

public:
#pragma region WeaponActionInterface
	virtual void ActivateAttack()   PURE_VIRTUAL(IWeaponInterface::ActivateAttack)
	virtual void DeactivateAttack() PURE_VIRTUAL(IWeaponInterface::DeactivateAttack)
#pragma endregion

#pragma region WeaponStateInterface
	virtual void EnableAttack()  PURE_VIRTUAL(IWeaponInterface::EnableAttack)
	virtual void DisableAttack() PURE_VIRTUAL(IWeaponInterface::DisableAttack)
#pragma endregion
};
