#include "TraceArrow.h"
#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/ArrowComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include "CPP_Portpolio.h"

UTraceArrow::UTraceArrow()
{
	for (int32 i = 0; i < EArrowType::Max; i++)
	{
		Arrows[i] = CreateDefaultSubobjectArray(Arrows, i);

		TraceMap.Add(EArrowType(i));
	}

	Arrows[EArrowType::Ceil]->SetRelativeLocation(FVector(0, 0, 80));
	Arrows[EArrowType::Floor]->SetRelativeLocation(FVector(0, 0, -80));

	Arrows[EArrowType::Left]->SetRelativeLocation(FVector(0, 50, 0));
	Arrows[EArrowType::Right]->SetRelativeLocation(FVector(0, -50, 0));

	Arrows[EArrowType::Land]->SetRelativeLocation(FVector(200, 0, 100));
	Arrows[EArrowType::Land]->SetRelativeRotation(FRotator(-90, 0, 0));

	Arrows[EArrowType::UpLeft]->SetRelativeLocation(FVector(40, -60, 60));
	Arrows[EArrowType::UpRight]->SetRelativeLocation(FVector(40, 60, 60));
}

void UTraceArrow::AttachArrows(ACharacter* Character)
{
	Owner = Character;

	for (int32 i = 0; i < EArrowType::Max; i++)
	{
		FAttachmentTransformRules const Rules(EAttachmentRule::KeepRelative, true);

		Arrows[i]->AttachToComponent(Owner->GetRootComponent(), Rules);
	}
}

bool UTraceArrow::CanHanging()
{
	TraceCenter();
	if (bCanHangingLedge)
	{
		bCanHangingLedge = false;
		return true;
	}

	return false;
}

bool UTraceArrow::CanClimbing()
{
	TraceCenter();
	if (bCanClimbingLedge)
	{
		bCanClimbingLedge = false;
		return true;
	}

	return false;
}

void UTraceArrow::TraceCenter()
{
	HitObstacle = nullptr;
	HitObstacleExtent = FVector::ZeroVector;
	HitDistance = 0;
	YawToFace = 0;

	WallLocation = FVector::ZeroVector;
	WallNormal = FVector::ZeroVector;

	TraceArrowLine(EArrowType::Center);

	auto const& Result = TraceMap[EArrowType::Center];

	if (Result.bBlockingHit && Result.GetActor() != nullptr)
	{
		TraceHeight();

		typedef UStaticMeshComponent MeshType;	// typedef 도 지역의 영향을 받는다

		if (MeshType const* const Mesh = Cast<MeshType>(Result.Actor->GetComponentByClass(MeshType::StaticClass())))
		{
			HitObstacle = Result.GetActor();

			FVector Min, Max;

			Mesh->GetLocalBounds(Min, Max);

			HitObstacleExtent = (Max - Min) * HitObstacle->GetActorScale();
			HitDistance = Result.Distance;
			YawToFace = UKismetMathLibrary::MakeRotFromX(-Result.Normal).Yaw;

			WallLocation = Result.Location;
			WallNormal = Result.Normal;
		}
	}
}

void UTraceArrow::TraceHeight()
{
	HeightLocation = FVector::ZeroVector;

	FVector Start = FVector(Owner->GetActorLocation().X, Owner->GetActorLocation().Y, Owner->GetActorLocation().Z + 500.0f)
		+ (Owner->GetActorRotation().Quaternion().GetForwardVector() * 30.0f);

	FVector End = FVector(Start.X, Start.Y, Start.Z - 500.0f);

	FHitResult HitResultWall;
	float Gap;

	if (UKismetSystemLibrary::SphereTraceSingle(
		Cast<UObject>(this),
		Start, End,
		20.0f,
		ETraceTypeQuery::TraceTypeQuery1,
		false, { Owner },
		EDrawDebugTrace::ForOneFrame,
		HitResultWall, true ))
	{
		HeightLocation = HitResultWall.Location;
		Gap = HeightLocation.Z - (Cast<ACharacter>(Owner)->GetMesh()->GetSocketLocation(PelvisSocket).Z);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow,
				FString::Printf(TEXT("Gap : %f"), Gap));

		if (UKismetMathLibrary::InRange_FloatFloat(Gap, 250.0f, 260.0f))
			bCanHangingLedge = true;
		if (UKismetMathLibrary::InRange_FloatFloat(Gap, 500.0f, 700.0f))
			bCanClimbingLedge = true;
	}
}

void UTraceArrow::TraceArrowLine(EArrowType const Type)
{
	auto* const Arrow = Arrows[Type];	// 누가 봐도 변수 타입이 정해져있는 경우 auto 를 사용할 예정

	FVector const Start = Arrow->GetComponentLocation();
	FVector const End = Start + (Arrow->GetForwardVector() * TraceDistance);

	UKismetSystemLibrary::LineTraceSingle
	(
		this,
		Start,
		End,
		UEngineTypes::ConvertToTraceType(ClimbCollision), // Can Climb TraceChannel Collision
		false,
		{ Owner },
		EDrawDebugTrace::ForOneFrame,
		TraceMap[Type],
		true
	);
}

void UTraceArrow::TraceArrowCapsule(EArrowType const Type)
{
	auto* const Arrow = Arrows[Type];

	FVector Start = Owner->GetActorLocation();
	FVector End = Start + FVector(Owner->GetActorForwardVector().X * 150.0f, Owner->GetActorForwardVector().Y * 150.0f, Owner->GetActorForwardVector().Z);

	// TODO : Check TraceChannel
	UKismetSystemLibrary::CapsuleTraceSingle
	(
		this,
		Start,
		End,
		20.0f,
		60.0f,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_EngineTraceChannel1), //ETraceTypeQuery::TraceTypeQuery1,
		false,
		{ Owner },
		EDrawDebugTrace::ForOneFrame,
		TraceMap[Type],
		false
	);
}