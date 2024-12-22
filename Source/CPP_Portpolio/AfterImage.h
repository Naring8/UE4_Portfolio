#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AfterImage.generated.h"

UCLASS()
class CPP_PORTPOLIO_API AAfterImage : public AActor
{
	GENERATED_BODY()
	
public:	
	AAfterImage();

	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
		void Init(class USkeletalMeshComponent* Pawn);

	void ImageActivate() {}

private:
	class UPoseableMeshComponent* PoseableMesh;
	class UMaterialInstance* GhostMaterial;
	TArray<class UMaterialInstanceDynamic*> Materials;

	bool IsSpawned = false;
	float FadeCountDown;
	float FadeOutTime = 0.5f;

};
