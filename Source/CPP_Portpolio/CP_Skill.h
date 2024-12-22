#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomStructs.h"
#include "CP_Skill.generated.h"

UCLASS(ABSTRACT)
class CPP_PORTPOLIO_API ACP_Skill : public AActor
{
	GENERATED_BODY()

public:
	ACP_Skill();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		FMontageData GetMontageData();

private:
#pragma region SkillProperties
	UPROPERTY(EditDefaultsOnly, Category = "ActionMontages")
		UAnimMontage* ActionMontage;
#pragma endregion

};
