#pragma once

#include "CoreMinimal.h"
#include <Engine/DataAsset.h>
#include "CustomStructs.h"
#include "MontageDataAsset.generated.h"

UCLASS()
class CPP_PORTPOLIO_API UMontageDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FMontageData GetMontageData(int32 const Index);

private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
		TArray<FMontageData> MontageData;
};
