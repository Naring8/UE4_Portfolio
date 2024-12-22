#pragma once

#include "CoreMinimal.h"
#include <Engine/DataTable.h>
#include <Components/ActorComponent.h>
#include "CharacterStateInterface.h"
#include "ParkourComponent.generated.h"

UENUM(BlueprintType)
namespace EParkour
{
	enum Type
	{
		None,
		Climb,
		Fall,
		Slide,
		Normal,
		Short,
		Wall,
		Max
	};
}
typedef EParkour::Type EParkourType;

USTRUCT(BlueprintType)
struct FParkourData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) TEnumAsByte<EParkour::Type> Type;
	UPROPERTY(EditAnywhere) class UAnimMontage* Montage;
	UPROPERTY(EditAnywhere) float PlayRate = 1.0f;
	UPROPERTY(EditAnywhere) FName Section;
	UPROPERTY(EditAnywhere) float DistMin;
	UPROPERTY(EditAnywhere) float DistMax;
	UPROPERTY(EditAnywhere) FVector Extent;
};

UCLASS(ClassGroup = (Athletics), meta = (BlueprintSpawnableComponent))
class CPP_PORTPOLIO_API UParkourComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UParkourComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
		bool BeginParkour();

	UFUNCTION()
		void EndParkour(UAnimMontage* const Montage, bool const bInterrupted);

private:

	FParkourData const* FindData(EParkourType const Type) const;

private:
#pragma region Parkour
	UPROPERTY(EditAnywhere)
		class UDataTable* DataTable;

	class AActor* Target;

	EParkourType ParkourType = EParkourType::None;
#pragma endregion

	class UTraceArrow* Arrows;
};