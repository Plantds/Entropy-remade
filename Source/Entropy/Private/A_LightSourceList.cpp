// Copyright (c) 2020 Tension Graphics AB


#include "A_LightSourceList.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AA_LightSourceList::AA_LightSourceList()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AA_LightSourceList::BeginPlay()
{
	Super::BeginPlay();
	AA_LightSourceList::list = this;
}

void AA_LightSourceList::EndPlay( const EEndPlayReason::Type type)
{
	Super::EndPlay(type);
	AA_LightSourceList::list = nullptr;
}

// Called every frame
void AA_LightSourceList::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AA_LightSourceList* AA_LightSourceList::GetRef()
{
	return list;
}

AA_LightSourceList* AA_LightSourceList::list = nullptr;

void AA_LightSourceList::AddLightPoint( AActor* actor )
{
	if( list == nullptr )
		return;
	list->lightPoints.Add( actor );
}

void AA_LightSourceList::RemoveLightPoint( AActor* actor )
{
	if( list == nullptr )
		return;
	list->lightPoints.Remove( actor );
}

const TArray< AActor* > AA_LightSourceList::GetLightPoints()
{
	if( list == nullptr )
		return {};
	return list->lightPoints;
}
