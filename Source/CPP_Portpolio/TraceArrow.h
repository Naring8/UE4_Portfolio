#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TraceArrow.generated.h"

namespace EArrow
{
	enum Type
	{
		Center,
		Ceil,
		Floor,
		Left,
		Right,
		UpLeft,
		UpRight,
		Land,
		Max	// enum의 갯수를 가져오기 위한 편의성 원소
	};
}
typedef EArrow::Type EArrowType;

UCLASS()
class CPP_PORTPOLIO_API UTraceArrow : public UObject
{
	GENERATED_BODY()

public:
	UTraceArrow();

	void AttachArrows(ACharacter* Character);

	bool CanHanging();
	bool CanClimbing();

	AActor* GetHitObstacle() { return HitObstacle; }

	FVector GetObstacleLocation() { return WallLocation; }
	FVector GetObstacleNormal() { return WallNormal; }
	FVector GetObstacleHeight() { return HeightLocation; }

	float GetObstacleYawToFace() { return YawToFace; }

private:
	void TraceCenter();
	void TraceHeight();

	void TraceArrowLine(EArrowType const Type);
	void TraceArrowCapsule(EArrowType const Type);

private:
#pragma region TraceArrow
	class UArrowComponent* Arrows[EArrowType::Max];
	TMap<EArrow::Type, FHitResult> TraceMap;

	float TraceDistance = 500.0f;
	float AcceptableAngle = 45.0f;

	AActor* HitObstacle;
	FVector HitObstacleExtent;
	float HitDistance;
	float YawToFace;
#pragma endregion

#pragma region Hang&Grab
	FVector HeightLocation;
	FVector WallLocation;
	FVector WallNormal;

	UPROPERTY(EditAnywhere, category = "Hang and Grab", meta = (AllowPrivateAccess))
		FName PelvisSocket;

	bool bCanClimbingLedge = false;
	bool bCanHangingLedge = false;
#pragma endregion

	bool bFalling;

private:
	class ACharacter* Owner;
	
};
