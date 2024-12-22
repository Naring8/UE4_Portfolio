#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponInterface.h"

#include <Components/CapsuleComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Particles/ParticleSystemComponent.h>

#include "CP_Skill.h"
#include "CustomStructs.h"
#include "CP_Weapon.generated.h"

UCLASS(Abstract)
class CPP_PORTPOLIO_API ACP_Weapon : public AActor, public IWeaponInterface
{
	GENERATED_BODY()

public:
	ACP_Weapon();

	FMontageData GetMontageData(int32 const Index);
	FMontageData GetSkillMontageData(int32 const Index);

protected:

public:
	virtual void ActivateAttack() override;
	virtual void DeactivateAttack() override;

	virtual void EnableAttack() override;
	virtual void DisableAttack() override;

protected: // private으로 바꿔도 될듯
#pragma region WeaponComponents
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
		class UParticleSystemComponent* Trail;
#pragma endregion

#pragma region SkillComponents
	UPROPERTY(EditDefaultsOnly, Category = "WeaponSkills")
		TSubclassOf<class ACP_Skill> SkillA;
	UPROPERTY(EditDefaultsOnly, Category = "WeaponSkills")
		TSubclassOf<class ACP_Skill> SkillB;
	UPROPERTY(EditDefaultsOnly, Category = "WeaponSkills")
		TSubclassOf<class ACP_Skill> SkillC;
#pragma endregion

private:
	UPROPERTY(EditDefaultsOnly, Category = "WeaponInformation", meta = (AllowPrivateAccess))
		float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponInformation", meta = (AllowPrivateAccess))
		FName SocketOnTrailStarted;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponInformation", meta = (AllowPrivateAccess))
		FName SocketOnTrailEnded;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataAssets", meta = (AllowPrivateAccess))
		class UMontageDataAsset* ActingData;	// replace to map(container)
};
