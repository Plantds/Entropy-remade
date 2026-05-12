// Copyright (c) 2020 Tension Graphics AB

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "AC_PlantRigSystem.generated.h"

USTRUCT()
struct FPlantStem
{
	GENERATED_BODY()
	UPROPERTY( EditAnywhere )
	FName boneName;
	UPROPERTY( EditAnywhere )
	float maxAngle = 30.0f;
	UPROPERTY( EditAnywhere )
	float speed = 1.0f;

	FQuat defaultRotation;
	FQuat startRot;
	FQuat endRot;
	FQuat noiceRot;
	float noiceDelay;
	float rotAlpha;
	int   boneIndex;
};

UENUM()
enum class EPlantCoordSystem : uint8
{
	Unreal,
	Maya
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ENTROPY_API UAC_PlantRigSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_PlantRigSystem();
	UPROPERTY( EditAnywhere )
	TArray< FPlantStem > stem;
	UPROPERTY( EditAnywhere )
	EPlantCoordSystem       coordSystem = EPlantCoordSystem::Unreal;
	UPoseableMeshComponent* poseableMesh;

	
	UPROPERTY( EditAnywhere )
	float range = 2000.0f;
	float speedMod;

	UPROPERTY( BlueprintReadOnly )
	bool isTracking = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void GenerateNoice();
};
