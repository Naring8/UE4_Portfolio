#include "CP_PlayerController.h"
#include "CharacterStateInterface.h"

void ACP_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveFB", this, &ThisClass::MoveFB);
	InputComponent->BindAxis("MoveLR", this, &ThisClass::MoveLR);
	InputComponent->BindAxis("LookLR", this, &ThisClass::LookLR);
	InputComponent->BindAxis("LookUD", this, &ThisClass::LookUD);

	InputComponent->BindAction("Dash", EInputEvent::IE_Pressed, this, &ThisClass::StartSprint);
	InputComponent->BindAction("Dash", EInputEvent::IE_Released, this, &ThisClass::EndSprint);

	InputComponent->BindAction("Mover", EInputEvent::IE_Pressed, this, &ThisClass::Mover);
	InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &ThisClass::Crouch);

	InputComponent->BindAction("Walk", EInputEvent::IE_Pressed, this, &ThisClass::Walk);

	InputComponent->BindAction("BaseAttack", EInputEvent::IE_Pressed, this, &ThisClass::BaseAttack);

	InputComponent->BindAction("BeginActionA", EInputEvent::IE_Pressed, this, &ThisClass::BeginActionA);

	InputComponent->BindAction("Interaction", EInputEvent::IE_Pressed, this, &ThisClass::Interaction);

}

#include "CharacterStateInterface.h"
// BindAxis Movement
void ACP_PlayerController::MoveFB(float const Value)
{
	if (auto* const ControlledPawn = GetPawn())
	{
		// auto* const ControlledPawnState = Cast<ICharacterStateInterface>(GetPawn());

		FVector const Direction = FRotator(0, GetControlRotation().Yaw, 0).Quaternion().GetForwardVector();

		ControlledPawn->AddMovementInput(Direction, Value);
	}
}

void ACP_PlayerController::MoveLR(float const Value)
{
	/*auto* const ControlledPawnState = Cast<ICharacterStateInterface>(GetPawn());
	if (ControlledPawnState->GetCharacterMovementState() != ECharacterMovementState::IDLE)
		return;*/

	if (auto* const ControlledPawn = GetPawn())
	{
		FVector const Direction = FRotator(0, GetControlRotation().Yaw, 0).Quaternion().GetRightVector();

		ControlledPawn->AddMovementInput(Direction, Value);
	}
}

void ACP_PlayerController::LookLR(float const Value)
{
	AddYawInput(Value);
}

void ACP_PlayerController::LookUD(float const Value)
{
	AddPitchInput(Value);
}

void ACP_PlayerController::Walk()
{
	auto* const ControlledPawnState = Cast<ICharacterStateInterface>(GetPawn());
	if (ControlledPawnState->GetCharacterMovementState() != ECharacterMovementState::IDLE)
		return;

	if (auto* const ControlledPawn = Cast<ICharacterActionInterface>(GetPawn()))
	{
		ControlledPawn->Walk();
	}
}

void ACP_PlayerController::Mover()
{
	if (auto* const ControlledPawn = Cast<ICharacterActionInterface>(GetPawn()))
	{
		if (auto* const CharacterState = Cast<ICharacterStateInterface>(GetPawn()))
			if (CharacterState->GetCharacterState() != ECharacterState::IDLE)
				return;

		ControlledPawn->Mover();
	}
}

void ACP_PlayerController::Crouch()
{
	if (auto* const ControlledPawn = Cast<ICharacterActionInterface>(GetPawn()))
	{
		ControlledPawn->Crouch();
	}
}

void ACP_PlayerController::StartSprint()
{
	if (auto* const ControlledPawn = Cast<ICharacterActionInterface>(GetPawn()))
	{
		ControlledPawn->StartSprint();
	}
}

void ACP_PlayerController::EndSprint()
{
	if (auto* const ControlledPawn = Cast<ICharacterActionInterface>(GetPawn()))
	{
		ControlledPawn->EndSprint();
	}
}

// BindAction Actions
void ACP_PlayerController::BaseAttack()
{
	auto* const ControlledPawnState = Cast<ICharacterStateInterface>(GetPawn());
	if (ControlledPawnState->GetCharacterMovementState() != ECharacterMovementState::IDLE)
		return;

	if (auto* const ControlledPawn = Cast<ICharacterActionInterface>(GetPawn()))
	{
		ControlledPawn->BaseAttack();
	}
}

void ACP_PlayerController::BeginActionA()
{
	auto* const ControlledPawnState = Cast<ICharacterStateInterface>(GetPawn());
	if (ControlledPawnState->GetCharacterMovementState() != ECharacterMovementState::IDLE)
		return;

	if (auto* const ControlledPawn = Cast<ICharacterActionInterface>(GetPawn()))
	{
		ControlledPawn->BeginActionA();
	}
}

void ACP_PlayerController::Interaction()
{
	auto* const ControlledPawnState = Cast<ICharacterStateInterface>(GetPawn());
	if (ControlledPawnState->GetCharacterMovementState() != ECharacterMovementState::IDLE)
		return;

	if (auto* const ControlledPawn = Cast<ICharacterActionInterface>(GetPawn()))
	{
		ControlledPawn->Interaction();
	}
}