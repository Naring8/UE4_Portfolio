#include "AN_MontageEnd.h"
#include "CharacterActionInterface.h"
#include "CharacterStateInterface.h"

void UAN_MontageEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (auto* const Character = Cast<ICharacterActionInterface>(MeshComp->GetOwner()))
	{
		Character->ExitLedge();
	}
}
