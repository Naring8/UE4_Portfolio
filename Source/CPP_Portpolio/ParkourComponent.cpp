#include "ParkourComponent.h"
#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/ArrowComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include "TraceArrow.h"
#include "CPP_Portpolio.h"

UParkourComponent::UParkourComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	//Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ThisClass::EndParkour);

	//if (DataTable != nullptr)
	//{
	//	TArray<FParkourData const*> Rows;

	//	DataTable->GetAllRows("", Rows);	// "" 안에 들어간 내용이 들어간 문자열만 받아옴 / "" << 아무것도 없기 때문에 모두 가져옴

	//	if (Rows.Num() > 0)
	//	{
	//		for (int32 Key = 0; Key < int32(EParkourType::Max); Key++)
	//		{
	//			TArray<FParkourData> Values;

	//			for (auto& Row : Rows)
	//				if (Key == Row->Type)
	//					Values.Add(*Row);

	//			DataMap.Add(EParkourType(Key), Values);
	//		}
	//	}
	//}
}

void UParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*TraceCenter();
	TraceHeight();

	if (HitObstacle != nullptr && !bIsHanging)
	{
		TraceArrowLine(EArrowType::Ceil);
		TraceArrowLine(EArrowType::Floor);
		TraceArrowLine(EArrowType::Left);
		TraceArrowLine(EArrowType::Right);
	}
	if (bIsHanging)
	{
		TraceArrowLine(EArrowType::UpLeft);
		TraceArrowLine(EArrowType::UpRight);
	}*/
}

bool UParkourComponent::BeginParkour()
{
	//if (ParkourType != EParkourType::None)	// 현재 Parkour를 수행 중일때
	//	return false;

	//FParkourData const* Data = nullptr;

	//if (CheckObstacle())	// 파쿠르를 가능케하는 조건 충족
	//{
	//	if (TraceMap[EArrowType::Ceil].bBlockingHit == true) // Climb
	//	{
	//		Data = FindData(EParkourType::Climb);
	//	}
	//	else if (TraceMap[EArrowType::Floor].bBlockingHit == false)	// Slide
	//	{
	//		Data = FindData(EParkourType::Slide);
	//	}
	//	else // else
	//	{
	//		for (int32 i = EParkourType::Normal; (i < EParkourType::Max); i++)
	//		{
	//			if ((Data = FindData(EParkourType(i))) != nullptr)
	//				break;
	//		}
	//	}
	//}

	//if (Data == nullptr)
	//	return false;

	//ParkourType = Data->Type;

	//if (ParkourType == EParkourType::Climb)
	//{
	//	Cast<ACharacter>(GetOwner())->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	//}
	//else
	//{
	//	Target = HitObstacle;
	//	Target->SetActorEnableCollision(false);
	//}

	//Cast<ACharacter>(GetOwner())->PlayAnimMontage(Data->Montage, Data->PlayRate, Data->Section);

	//GetOwner()->SetActorRotation(FRotator(0, YawToFace, 0));

	return true;
}

void UParkourComponent::EndParkour(UAnimMontage* const Montage, bool const bInterrupted)
{
	/*if (auto* const Character = Cast<ACharacter>(GetOwner()))
	{
		switch (ParkourType)
		{
		case EParkourType::Climb:
		{
			Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			break;
		}
		case EParkourType::Wall:
		{
			Target->SetActorEnableCollision(true);
			Target = nullptr;
			break;
		}

		ParkourType = EParkourType::None;
		}
	}*/
}

#include "Kismet/KismetSystemLibrary.h"
#include "Math/Vector.h"

FParkourData const* UParkourComponent::FindData(EParkourType const Type) const
{
	/*if (DataMap.Contains(Type) == false)
		return false;

	auto const& DataArray = DataMap[Type];

	for (int32 i = 0; i < DataArray.Num(); i++)
	{
		if (DataArray[i].DistMin <= HitDistance && HitDistance <= DataArray[i].DistMax)
		{
			bool bResult = true;

			for (int32 j = 0; j < 3; j++)
			{
				if (DataArray[i].Extent[j] == 0.0f)
					continue;

				bResult &= FMath::IsNearlyEqual(DataArray[i].Extent[j], HitObstacleExtent[j], 10);
			}

			if (bResult == true)
				return &DataArray[i];
		}
	}*/

	return nullptr;
}