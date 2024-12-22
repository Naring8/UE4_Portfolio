#include "ANS_AttackEnabled.h"
#include "WeaponInterface.h"

void UANS_AttackEnabled::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (auto* const Character = Cast<IWeaponInterface>(MeshComp->GetOwner()))
	{
		Character->EnableAttack();
	}
}

void UANS_AttackEnabled::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (auto* const Character = Cast<IWeaponInterface>(MeshComp->GetOwner()))
	{
		Character->DisableAttack();
	}
}

