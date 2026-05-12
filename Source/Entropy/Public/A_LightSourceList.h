// Copyright (c) 2020 Tension Graphics AB

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A_LightSourceList.generated.h"

UCLASS()
class ENTROPY_API AA_LightSourceList : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AA_LightSourceList();

private:

	static AA_LightSourceList* list;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay( const EEndPlayReason::Type) override;

public:	
	TArray< AActor* > lightPoints;
	// Called every frame
	virtual void Tick( float DeltaTime ) override;
	static AA_LightSourceList* GetRef();

	UFUNCTION( BlueprintCallable, Category = "LightList" )
	static void AddLightPoint( AActor* actor );
	UFUNCTION( BlueprintCallable, Category = "LightList" )
	static void RemoveLightPoint( AActor* actor );
	UFUNCTION( BlueprintCallable, Category = "LightList" )
	static const TArray<AActor*> GetLightPoints();
};
