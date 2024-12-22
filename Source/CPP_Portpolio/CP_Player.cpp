#include "CP_Player.h"

#include "ParkourComponent.h"
//#include <Camera/CameraComponent.h>
//#include <GameFramework/SpringArmComponent.h>

ACP_Player::ACP_Player()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bUsePawnControlRotation = true;

	SpringArm->TargetArmLength = 480.0f;
	SpringArm->TargetOffset = { 0.0f, 0.0f, 75.0f };

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	Camera->SetFieldOfView(80);
}
