#include "CP_Character.h"
#include <Curves/CurveFloat.h>
#include <Components/TimelineComponent.h>
#include "ParkourComponent.h"
#include "CustomStructs.h"
#include "TraceArrow.h"
#include "CP_Weapon.h"

ACP_Character::ACP_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	// 이동 방향으로 회전
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->JumpZVelocity = 500.0f;

	DashTimeline = CreateDefaultSubobject<UTimelineComponent>("ZoomTimeline");
	Arrows = CreateDefaultSubobject<UTraceArrow>("TraceArrows");
}

void ACP_Character::OnConstruction(FTransform const& Transform)
{
	Super::OnConstruction(Transform);
}

void ACP_Character::BeginPlay()
{
	Super::BeginPlay();

#pragma region AttachWeapons
	FActorSpawnParameters SpawnParameters;

	SpawnParameters.Owner = SpawnParameters.Instigator = this;

	for (auto& WeaponClass : WeaponClasses)
	{
		if (!IsValid(WeaponClass))
			continue;

		auto* const SpawnedWeapon = GetWorld()->SpawnActor<ACP_Weapon>(WeaponClass, SpawnParameters);

		FAttachmentTransformRules const Rules(EAttachmentRule::SnapToTarget, true);

		SpawnedWeapon->AttachToComponent(GetMesh(), Rules, WeaponSocket);

		Weapons.Add(SpawnedWeapon);
	}

	if (Weapons.Num() > 0)
	{
		Weapon = Weapons[0];
		Weapon->DisableAttack();
	}

#pragma endregion

	InitCharacterState();

#pragma region SetSprintCurves
	//if (SprintCurve != nullptr)
	//{
	//	FOnTimelineFloat OnDash;

	//	OnDash.BindDynamic(this, &ThisClass::Dash);
	//	DashTimeline->AddInterpFloat(SprintCurve, OnDash);	// Dash 함수를 계속 부르면서 BlendCurve에 있는 값이 인자(Alpha)로 넘어감
	//	//DashTimeline->SetPlayRate(1 / SprintCurve);
	//}
#pragma endregion

#pragma region Arrows
	Arrows->AttachArrows(this);
#pragma endregion

	// OnMontageEnded
	GetMesh()->GetAnimInstance()->OnMontageBlendingOut.AddDynamic(this, &ThisClass::AttackContinued);
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ThisClass::AttackEnded);
}

void ACP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckCharacter();

	if (CheckGrabLedge())
		GrabLedge();

	CheckClimbing();


	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow,
			FString::Printf(TEXT("Character Movement State : %d"), CharacterMovementState));*/
}

void ACP_Character::ActivateAttack()
{
	if (auto* const CurrentWeapon = Cast<IWeaponInterface>(Weapon))
		CurrentWeapon->ActivateAttack();
}

void ACP_Character::DeactivateAttack()
{
	if (auto* const CurrentWeapon = Cast<IWeaponInterface>(Weapon))
		CurrentWeapon->DeactivateAttack();
}

void ACP_Character::EnableAttack()
{
	if (auto* const CurrentWeapon = Cast<IWeaponInterface>(Weapon))
		CurrentWeapon->EnableAttack();

	bAttackEnabled = true;
}

void ACP_Character::DisableAttack()
{
	if (auto* const CurrentWeapon = Cast<IWeaponInterface>(Weapon))
		CurrentWeapon->DisableAttack();

	bAttackEnabled = false;
}

void ACP_Character::InitCharacterState()
{
	CharacterState = ECharacterState::IDLE;
	CharacterMovementState = ECharacterMovementState::IDLE;

	bIsHanging = false;
	bCanClimb = false;

	// CurHP = MaxHP;
	// CurMP = MaxMP;
}

void ACP_Character::AttackContinued(UAnimMontage* const Montage, bool const bInterrupted)
{
	if (bAttackTransisted)
	{
		BaseAttack();
	}
}

void ACP_Character::AttackEnded(UAnimMontage* const Montage, bool const bInterrupted)
{
	if (!bInterrupted)
	{
		SetCharacterState(ECharacterState::IDLE);
		bAttackEnabled = true;
		AttackIndex = 0;
	}
}

void ACP_Character::Walk()
{
	if (CharacterState != ECharacterState::IDLE)
		return;

	if (CharacterMovementState == ECharacterMovementState::IDLE)
	{
		CharacterMovementState = ECharacterMovementState::WALK;
		SetCharacterSpeed(WalkSpeed);
	}
	else
	{
		CharacterMovementState = ECharacterMovementState::IDLE;
		SetCharacterSpeed(RunSpeed);
	}
}

void ACP_Character::Mover()
{
	if (bCanClimb)
		ClimbWall();
	else if (bIsHanging)
	{
		ClimbLedge();
		return;
	}

	if (CharacterState != ECharacterState::IDLE || CharacterMovementState != ECharacterMovementState::IDLE)
		return;

	Jump();
	CharacterMovementState = ECharacterMovementState::JUMP;
}

void ACP_Character::Crouch()
{
	if (bIsHanging)
		ExitLedge();

	if (auto* const CharacterAnim = Cast<ICharacterActionInterface>(GetMesh()->GetAnimInstance()))
	{
		CharacterAnim->Crouch();
		if (CharacterMovementState == ECharacterMovementState::CROUCH)
		{
			SetCharacterSpeed(RunSpeed);
			CharacterMovementState = ECharacterMovementState::IDLE;
			return;
		}
		SetCharacterSpeed(WalkSpeed);
		CharacterMovementState = ECharacterMovementState::CROUCH;
	}
}

void ACP_Character::Dash(float const Alpha)
{
	GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(RunSpeed, SprintSpeed, Alpha);
	//SpringArm->TargetArmLength = FMath::Lerp(TargetArmLength, TargetArmLengthOnZoomIn, Alpha);
	//SpringArm->SocketOffset = FMath::Lerp(SocketOffset, SocketOffsetOnZoomIn, Alpha);
	//Camera->SetFieldOfView(FMath::Lerp(FieldOfView, FieldOfViewOnZoomIn, Alpha));
}

// 연속 공격
void ACP_Character::BaseAttack()
{
	// TODO : 플레이어가 공중에 있을 때. 애니메이션이 DefualtGroup(몸 전체)여서 중간에 점프가 끊기는 현상
	if (bAttackTransisted)
	{
		bAttackTransisted = false;
	}
	else
	{
		if (!bAttackEnabled)
			return;

		if (CharacterState == ECharacterState::ACTING)
		{
			bAttackTransisted = true;
			bAttackEnabled = false;
			return;
		}
		else if (CharacterState != ECharacterState::IDLE)
			return;
	}

	bAttackEnabled = false;

	SetCharacterState(ECharacterState::ACTING);

	FMontageData const MontageData = Weapon->GetMontageData(AttackIndex++);
	PlayAnimMontage(MontageData.Montage, MontageData.PlayRate, MontageData.Section);
}

void ACP_Character::BeginActionA()
{
	if (CharacterState != ECharacterState::IDLE)
		return;

	SetCharacterState(ECharacterState::ACTING);

	FMontageData const MontageData = Weapon->GetSkillMontageData(1);
	PlayAnimMontage(MontageData.Montage, MontageData.PlayRate, MontageData.Section);
}

void ACP_Character::StartSprint()
{
	DashTimeline->Play();
}

void ACP_Character::EndSprint()
{
	DashTimeline->Reverse();
}

void ACP_Character::TakeDown()
{
	if (IsValid(TakeDownedActor))
	{

	}
}

#include <Kismet/KismetSystemLibrary.h>

void ACP_Character::GrabLedge()
{
	if (auto* const CharacterAnim = Cast<ICharacterActionInterface>(GetMesh()->GetAnimInstance()))
	{
		bIsHanging = true;
		CharacterAnim->CanGrab(bIsHanging);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		CharacterMovementState = ECharacterMovementState::HANG;

		// Not to pass the wall . Adjust value
		FVector WallNormal = Arrows->GetObstacleNormal();
		FVector WallLocation = Arrows->GetObstacleLocation();
		FVector WallHeight = Arrows->GetObstacleHeight();

		float YawToFace = Arrows->GetObstacleYawToFace();

		FVector WallVector = FVector((WallNormal.X * 22.0f) + WallLocation.X, (WallNormal.Y * 22.0f) + WallLocation.Y, WallHeight.Z - 120.0f);

		FVector RelativeLocation = WallVector;
		FRotator RelativeRotation = FVector(WallNormal.X, WallNormal.Y, WallNormal.Z - 180.0f).ToOrientationQuat().Rotator();
		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, WallVector.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, RelativeLocation.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, RelativeRotation.ToString());
		}*/
		FLatentActionInfo temp;
		UKismetSystemLibrary::MoveComponentTo
		(
			GetCapsuleComponent(),
			RelativeLocation, RelativeRotation,
			false, false, 0.13f, false,
			EMoveComponentAction::Stop, temp
		);

		SetActorRotation(FRotator(0, YawToFace, 0));

		GetCharacterMovement()->StopMovementImmediately();
	}
}

void ACP_Character::ExitLedge()
{
	if (auto* const CharacterAnim = Cast<ICharacterActionInterface>(GetMesh()->GetAnimInstance()))
	{
		InitCharacterState();

		CharacterAnim->CanGrab(bIsHanging);
		CharacterAnim->SetClimbLedge(bIsHanging);

		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void ACP_Character::ClimbLedge()
{
	if (auto* const CharacterAnim = Cast<ICharacterActionInterface>(GetMesh()->GetAnimInstance()))
	{
		CharacterAnim->SetClimbLedge(bIsHanging);
	}
}

void ACP_Character::ClimbWall()
{
	if (auto* const CharacterAnim = Cast<ICharacterActionInterface>(GetMesh()->GetAnimInstance()))
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		CharacterMovementState = ECharacterMovementState::JUMP;
		CharacterAnim->ClimbWall();
	}
}

void ACP_Character::Interaction()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
			FString::Printf(TEXT("T Pressed Character")));
}

#include <Kismet/KismetSystemLibrary.h>
bool ACP_Character::CheckCharacter()
{
	if (CharacterMovementState != ECharacterMovementState::CROUCH && CharacterMovementState != ECharacterMovementState::WALK)
		return false;

	FVector const Start = GetActorLocation() + FVector(0, 0, 75.0f);
	FVector const End = Start + (GetActorRotation().Quaternion().GetForwardVector() * 100.0f);

	FHitResult HitResult;
	bool bIsTraceSuccess = UKismetSystemLibrary::SphereTraceSingle
	(
		this,
		Start,
		End,
		25.0f,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn),
		false,
		{ GetOwner() },
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	TakeDownedActor = HitResult.Actor.Get();

	return bIsTraceSuccess;
}

bool ACP_Character::CheckGrabLedge()
{
	if (Arrows->CanHanging() &&
		CharacterMovementState == ECharacterMovementState::JUMP)
		return true;

	return false;
}

bool ACP_Character::CheckClimbing()
{
	if (Arrows->CanClimbing())
	{
		bCanClimb = true;
		return true;
	}

	bCanClimb = false;
	return false;
}

//void UParkourComponent::GrabLedge()
//{
//	if (auto* const Owner = Cast<ACharacter>(GetOwner()))
//	{
//		if (auto* const AnimInstance = Cast<IParkourInterface>(Owner->GetMesh()->GetAnimInstance()))
//		{
//			AnimInstance->CanGrab(true);
//
//			Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
//			bIsHanging = true;
//
//			// Not to pass the wall . Adjust value 
//			FVector WallVector = (WallNormal * FVector(42.5f, 42.5f, 0.0f)) + WallLocation;
//
//			FVector RelativeLocation = FVector(WallVector.X, WallVector.Y, HeightLocation.Z - 300.0f);
//			FRotator RelativeRotation = FVector(WallNormal.X, WallNormal.Y, WallNormal.Z - 180.0f).ToOrientationQuat().Rotator();
//			FLatentActionInfo temp;
//			UKismetSystemLibrary::MoveComponentTo
//			(
//				Owner->GetCapsuleComponent(),
//				RelativeLocation, RelativeRotation,
//				false, false, 0.13f, false,
//				EMoveComponentAction::Stop, temp
//			);
//
//			Owner->SetActorRotation(FRotator(0, YawToFace, 0));
//
//			Owner->GetCharacterMovement()->StopMovementImmediately();
//			if (auto* const PlayerController = Cast<IParkourInterface>(Owner->GetController()))
//				PlayerController->CanGrab(bIsHanging);
//		}
//	}
//}
//
//void UParkourComponent::ClimbLedge()
//{
//	if (bIsClimbingLedge)
//		return;
//
//	if (auto* const Owner = Cast<ACharacter>(GetOwner()))
//	{
//		if (auto* const AnimInstance = Cast<IParkourInterface>(Owner->GetMesh()->GetAnimInstance()))
//		{
//			AnimInstance->SetClimbLedge(true);
//			Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
//
//			bIsClimbingLedge = true;
//			bIsHanging = false;
//
//			if (auto* const PlayerController = Cast<IParkourInterface>(Owner->GetController()))
//				PlayerController->CanGrab(bIsHanging);
//		}
//	}
//}
//
//void UParkourComponent::SetClimbLedge(bool IsClimbing)
//{
//	bIsClimbingLedge = IsClimbing;
//
//	if (auto* const Owner = Cast<ACharacter>(GetOwner()))
//		Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
//}
//
//void UParkourComponent::ExitLedge()
//{
//	if (auto* const Owner = Cast<ACharacter>(GetOwner()))
//	{
//		Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
//		bIsHanging = false;
//
//		if (auto* const AnimInstance = Cast<IParkourInterface>(Owner->GetMesh()->GetAnimInstance()))
//			AnimInstance->CanGrab(false);
//
//		if (auto* const PlayerController = Cast<IParkourInterface>(Owner->GetController()))
//			PlayerController->CanGrab(false);
//	}
//}

