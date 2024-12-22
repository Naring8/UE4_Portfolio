#include "CP_Skill.h"

ACP_Skill::ACP_Skill()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACP_Skill::BeginPlay()
{
	Super::BeginPlay();

}

void ACP_Skill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FMontageData ACP_Skill::GetMontageData()
{
	FMontageData AnimMontage;
	AnimMontage.Montage = ActionMontage;

	return AnimMontage;
}

