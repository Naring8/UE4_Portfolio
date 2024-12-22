#include "CP_Weapon.h"
#include "CP_Skill.h"
#include "MontageDataAsset.h"

ACP_Weapon::ACP_Weapon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	Capsule->SetupAttachment(Mesh);

	Trail = CreateDefaultSubobject<UParticleSystemComponent>("Trail");
	Trail->SetupAttachment(Mesh);
	Trail->SetAutoActivate(false);
}

FMontageData ACP_Weapon::GetMontageData(int32 const Index)
{
	FMontageData MontageData;

	// replace to map finder
	if (IsValid(ActingData))
		MontageData = ActingData->GetMontageData(Index);

	return MontageData;
}

FMontageData ACP_Weapon::GetSkillMontageData(int32 const Index)
{
	FMontageData MontageData;

	if (!IsValid(SkillA))
		return MontageData;

	if (Index == 1)
		MontageData = SkillA.GetDefaultObject()->GetMontageData();
	/*else if (Index == 2)
		MontageData = SkillB->GetMontageData();
	else if (Index == 3)
		MontageData = SkillC->GetMontageData();*/

	return MontageData;
}

void ACP_Weapon::ActivateAttack()
{
	Trail->BeginTrails(SocketOnTrailStarted, SocketOnTrailEnded, ETrailWidthMode::ETrailWidthMode_FromSecond, 1.0f);

	EnableAttack();
}

void ACP_Weapon::DeactivateAttack()
{
	Trail->EndTrails();

	DisableAttack();
}

void ACP_Weapon::EnableAttack()
{
	Mesh->SetVisibility(true);
	Capsule->SetGenerateOverlapEvents(true);
}

void ACP_Weapon::DisableAttack()
{
	Mesh->SetVisibility(false);
	Capsule->SetGenerateOverlapEvents(false);
}