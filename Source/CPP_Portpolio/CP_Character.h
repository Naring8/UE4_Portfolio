#pragma once

#include "CoreMinimal.h"
#include <GameFramework/Character.h>

#include "CharacterActionInterface.h"
#include "CharacterStateInterface.h"
#include "WeaponInterface.h"

#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

#include "CP_Character.generated.h"

UCLASS(Abstract)
class CPP_PORTPOLIO_API ACP_Character : public ACharacter, public ICharacterActionInterface, public ICharacterStateInterface, public IWeaponInterface
{
	GENERATED_BODY()

public:
	ACP_Character();

	virtual void OnConstruction(FTransform const& Transform) override;
protected:
	virtual void BeginPlay() override;

private:
	void SetCharacterSpeed(float Speed) { GetCharacterMovement()->MaxWalkSpeed = Speed; }

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void AttackContinued(UAnimMontage* const Montage, bool const bInterrupted);
	UFUNCTION()
		void AttackEnded(UAnimMontage* const Montage, bool const bInterrupted);

private:
#pragma region ICharacterActionInterface
	virtual void Walk() override;
	virtual void Mover() override;
	virtual void Crouch() override;
	
	virtual void StartSprint() override;
	virtual void EndSprint() override;

	virtual void BaseAttack() override;

	virtual void BeginActionA() override;

	virtual void Interaction() override;
	virtual void TakeDown() override;

	virtual void GrabLedge() override;
	virtual void ExitLedge() override;
	virtual void ClimbLedge() override;

	virtual void ClimbWall() override;
#pragma endregion


#pragma region IWeaponInterface
	virtual void ActivateAttack() override;
	virtual void DeactivateAttack() override;

	virtual void EnableAttack() override;
	virtual void DisableAttack() override;
#pragma endregion

#pragma region ICharacterStateInterface
	virtual void InitCharacterState() override;
	virtual void SetCharacterState(ECharacterState State) override { CharacterState = State; }
	virtual ECharacterState GetCharacterState() override { return CharacterState; }
#pragma endregion

#pragma region ICharacterMovementStateInterface
	virtual void SetCharacterMovementState(ECharacterMovementState MovementState) override { CharacterMovementState = MovementState; }
	virtual ECharacterMovementState GetCharacterMovementState() override { return CharacterMovementState; }
#pragma endregion

	void Dash(float const Alpha);

public:
	UFUNCTION(BlueprintCallable)
		bool CheckCharacter();
	bool CheckGrabLedge();
	bool CheckClimbing();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Information", meta = (AllowPrivateAccess))
		float MaxHP;
	UPROPERTY(EditDefaultsOnly, Category = "Information", meta = (AllowPrivateAccess))
		float MaxMP;

private:
#pragma region Character State
	ECharacterState CharacterState = ECharacterState::IDLE;
	ECharacterMovementState CharacterMovementState = ECharacterMovementState::IDLE;

	float CurHP;
	float CurMP;
#pragma endregion

private:
#pragma region Weapons
	UPROPERTY(EditDefaultsOnly, Category = "Weapons", meta = (AllowPrivateAccess))
		TArray<TSubclassOf<class ACP_Weapon>> WeaponClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons", meta = (AllowPrivateAccess))
		FName WeaponSocket;

	class ACP_Weapon* Weapon;

	TArray<decltype(Weapon)> Weapons;

	int32 AttackIndex = 0;
#pragma endregion

#pragma region Sprint
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
		class UCurveFloat* SprintCurve;
	UPROPERTY()
		class UTimelineComponent* DashTimeline;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
		float WalkSpeed = 300.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
		float RunSpeed = 450.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
		float SprintSpeed = 650.0f;
#pragma endregion

	bool bAttackTransisted = false;
	bool bAttackEnabled = true;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
		AActor* TakeDownedActor = nullptr;

#pragma region Vaulting
	bool bIsHanging = false;
	bool bCanClimb = false;
#pragma endregion

	class UTraceArrow* Arrows;
};
