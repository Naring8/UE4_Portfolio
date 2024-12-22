#include "ANS_AttackActivated.h"
#include "WeaponInterface.h"

void UANS_AttackActivated::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (auto* const Character = Cast<IWeaponInterface>(MeshComp->GetOwner()))
	{
		Character->ActivateAttack();
	}
}

void UANS_AttackActivated::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (auto* const Character = Cast<IWeaponInterface>(MeshComp->GetOwner()))
	{
		Character->DeactivateAttack();
	}
}
