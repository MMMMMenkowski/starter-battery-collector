// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Engine/EngineTypes.h"
#include "SpawnVolume.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	// Find a random point within the BoxComponent.
	UFUNCTION(BlueprintPure, Category = "Spawning")
	FVector GetRandomPointVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// using TSubclassOf means there enforce the only APickup and its children. which used as the ptr so can be forward declaration.
	// edit whattospawn directly in detail panel. 
	UPROPERTY(EditAnywhere, category="Spawning")
	TSubclassOf<class APickup> WhatToSpawn;

	// dont be a property so this dont need UPROPERTY.
	FTimerHandle SpawnTimer;

	// the range spawn delay can be.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Spawning")
	float SpawnDelayLow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Spawning")
	float SpawnDelayHigh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Return the where2spawn subobject.
	FORCEINLINE class UBoxComponent* GetWhereToSpawn() const { return WhereToSpawn; }

	// getter and setter of active state only in code side.
	bool GetState();
	void SetState(bool State);

private:
	// BoxComponent to determine where pickups should be spawned.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", Meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* WhereToSpawn;
	
	// handle spawning a new pickup
	void SpawnPickup();

	// set the actual delaytime but that can used only in code side.
	float SpawnDelay;

	// whether or not the spawnVolume to spawn batteries
	bool bIsActive;
};
