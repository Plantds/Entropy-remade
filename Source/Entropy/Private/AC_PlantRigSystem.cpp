// Copyright (c) 2020 Tension Graphics AB


#include "AC_PlantRigSystem.h"
#include "Kismet/GameplayStatics.h"
#include "KismetAnimationLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "A_LightSourceList.h"
#include "SkeletalMeshSocket.generated.h"
#include "GameFramework/Character.h"


// Sets default values for this component's properties
UAC_PlantRigSystem::UAC_PlantRigSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//posableMesh                       = CreateDefaultSubobject< UPoseableMeshComponent >( TEXT( "PosebleMesh" ) );

	// ...
}

// Called when the game starts
void UAC_PlantRigSystem::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->SetActorScale3D( GetOwner()->GetActorScale() * FVector( FMath::FRandRange( 0.8f, 1.0f ) ) );
	GetOwner()->SetActorRotation( GetOwner()->GetActorRotation().Quaternion() * FRotator( 0.0f, FMath::FRandRange( -180.f, 180.f ), 0.0f ).Quaternion() );
	// ...
	speedMod     = FMath::FRandRange(0.5f, 1.5f);
	poseableMesh = GetOwner()->GetComponentByClass< UPoseableMeshComponent >();

	if( !poseableMesh )
	{
		if( GEngine )
			GEngine->AddOnScreenDebugMessage( INDEX_NONE, 0, FColor::Red, "No skeleton!!!" );
		return;
	}
	//posableMesh->SetupAttachment( RootComponent );
	//posableMesh->SetSkeletalMesh( skeletalComponent->GetSkeletalMeshAsset() );
	for( int i = 1; i < stem.Num(); i++ )
	{
		FTransform boneWorld      = poseableMesh->GetBoneTransform( stem[ i ].boneName, ERelativeTransformSpace::RTS_World );
		stem[ i ].defaultRotation = boneWorld.GetRotation();
		stem[ i ].startRot        = stem[ i ].defaultRotation;
		stem[ i ].endRot          = stem[ i ].defaultRotation;
		stem[ i ].rotAlpha        = 0.0f;
	}
}


// Called every frame
void UAC_PlantRigSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if( !poseableMesh )
	{
		if( GEngine )
			GEngine->AddOnScreenDebugMessage( INDEX_NONE, 0, FColor::Red, "No skeleton!!!" );
		return;
	}
	if( !stem.Num() )
	{
		if( GEngine )
			GEngine->AddOnScreenDebugMessage( INDEX_NONE, 0, FColor::Red, "No rig data!!!" );
		return;
	}
	/* ACharacter* character = UGameplayStatics::GetPlayerCharacter( GetWorld(), 0 );
	FVector Forwards = character->GetActorForwardVector();
	FVector dir = character->GetActorLocation() - GetOwner()->GetActorLocation();
	dir.Normalize();
	float val = FVector::DotProduct( Forwards, dir );
	if( val >= 0 )
	{
		GetOwner()->SetActorHiddenInGame( true );
		//poseableMesh->SetComponentTickEnabled( false );
		poseableMesh->SetVisibility( false );
	}
	else
	{
		GetOwner()->SetActorHiddenInGame( false );
		//poseableMesh->SetComponentTickEnabled( true );
		poseableMesh->SetVisibility( true );
	}*/

	float rotAlpha = 0.5f * DeltaTime;
	for ( int i = 1; i < stem.Num(); i++ )
	{
		stem[ i ].noiceDelay -= DeltaTime;
		if( stem[ i ].noiceDelay <= 0.0f )
		{
			if( coordSystem == EPlantCoordSystem::Maya )
				stem[ i ].noiceRot = FRotator( 0.0f, FMath::FRandRange( -10.0f, 10.0f ), FMath::FRandRange( -10.0f, 10.0f ) ).Quaternion();
			else
				stem[ i ].noiceRot = FRotator( FMath::FRandRange( -10.0f, 10.0f ), FMath::FRandRange( -10.0f, 10.0f ), 0.0f ).Quaternion();
			stem[ i ].noiceDelay = FMath::FRandRange( 1.0f, 5.0f );
		}
	}
	AActor*             closestActor = nullptr;
	float               length       = range*range;
	AA_LightSourceList* list         = AA_LightSourceList::GetRef();
	if( list )
	{
		for( int i = 0; i < list->lightPoints.Num(); i++ )
		{
			float newLength = ( list->lightPoints[ i ]->GetActorLocation() - GetOwner()->GetActorLocation() ).SquaredLength();
			if( newLength < length )
			{
				closestActor = list->lightPoints[ i ];
				length       = newLength;
			}
		}
	}
	if( !closestActor )
	{
		isTracking = false;
		for( int i = 1; i < stem.Num(); i++ )
		{
			FTransform socketWorld = poseableMesh->GetBoneTransform( stem[ i ].boneName, ERelativeTransformSpace::RTS_World );

			FQuat resultRot = FQuat::Slerp( socketWorld.GetRotation(), stem[ i ].defaultRotation * stem[ i ].noiceRot, std::min( (stem[ i ].speed*speedMod) * rotAlpha, 1.0f ) );
			poseableMesh->SetBoneRotationByName( stem[ i ].boneName, FRotator( resultRot ), EBoneSpaces::Type::WorldSpace );
		}
		return;
	}
	isTracking = true;
	//UAC_PlantRigSystem::lightPoints;
	//APawn* pawn = UGameplayStatics::GetPlayerPawn( GetWorld(), 0 );
	//const FTransform& playerWorld = pawn->GetTransform();
	const FTransform& itemWorld = closestActor->GetTransform();

	FTransform parentWorld     = poseableMesh->GetBoneTransform( stem[ 0 ].boneName, ERelativeTransformSpace::RTS_World );
	FTransform invParentWorld  = parentWorld.Inverse();
	FVector front = FVector( 0, 0, 1 );
	if (coordSystem == EPlantCoordSystem::Maya)
		front = FVector( 0, -1, 0 );
	for (int i = 1; i < stem.Num(); i++)
	{
		FTransform socketWorld = poseableMesh->GetBoneTransform( stem[ i ].boneName, ERelativeTransformSpace::RTS_World );

		FTransform LocalSocketTransform = socketWorld * invParentWorld;
		FTransform LocalplayerTransform = itemWorld * invParentWorld;
		FTransform result               = UKismetAnimationLibrary::K2_LookAt( LocalSocketTransform, LocalplayerTransform.GetLocation(), front, false, {}, stem[ i ].maxAngle );

		FTransform newWorld = result * parentWorld;
		FQuat      resultRot = FQuat::Slerp( socketWorld.GetRotation(), newWorld.GetRotation() * stem[ i ].noiceRot, std::min( ( stem[ i ].speed * speedMod ) * rotAlpha, 1.0f ) );
		poseableMesh->SetBoneRotationByName( stem[ i ].boneName, FRotator( resultRot ), EBoneSpaces::Type::WorldSpace );

		if( i != stem.Num() - 1 )
		{
			parentWorld    = newWorld;
			invParentWorld = parentWorld.Inverse();
		}
	}
}

void UAC_PlantRigSystem::GenerateNoice()
{
	for( int i = 1; i < stem.Num(); i++ )
	{
		stem[i].noiceRot = FRotator( FMath::FRandRange( -10.0f, 10.0f ),FMath::FRandRange( -10.0f, 10.0f ), 0).Quaternion();
	}
}

