#pragma once

#include "CoreMinimal.h"
#include "CustomStructs.generated.h"

class CPP_PORTPOLIO_API CustomStructs
{
};

USTRUCT(Atomic, BlueprintType)
struct FMontageData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UAnimMontage* Montage = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float PlayRate = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FName Section;
};
