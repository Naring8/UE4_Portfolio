#pragma once

#include "CoreMinimal.h"
#include <Animation/AnimInstance.h>
#include "CharacterActionInterface.h"
#include "CP_PlayerAnimInstance.generated.h"

UCLASS()
class CPP_PORTPOLIO_API UCP_PlayerAnimInstance : public UAnimInstance, public ICharacterActionInterface
{
	GENERATED_BODY()

public:
	void NativeUpdateAnimation(float const DeltaSeconds) override;

	bool const IsCharacterHanging() { return bIsHanging; }

private:
#pragma region ICharacterActionInterface
	virtual void Crouch() override;

	virtual void StartSprint() override {};
	virtual void EndSprint() override {};

	virtual void BaseAttack() override {};

	virtual void BeginActionA() override {};

	virtual void CanGrab(bool CanGrabBI) override { bIsHanging = CanGrabBI; }
	virtual void SetClimbLedge(bool IsClimbing) override;

	virtual void ClimbWall() override;
#pragma endregion

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
		float Direction;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
		float Speed;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
		float Yaw;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
		float Pitch;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
		bool bIsJumped = false;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
		bool bIsHanging = false;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
		bool bIsClimbing = false;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
		bool bIsCrouch = false;

	UPROPERTY(EditAnywhere, category = "AnimationMontage", meta = (AllowPrivateAccess))
		class UAnimMontage* ClimbLedgeMontage = nullptr;
	UPROPERTY(EditAnywhere, category = "AnimationMontage", meta = (AllowPrivateAccess))
		class UAnimMontage* ClimbWallMontage = nullptr;
};
