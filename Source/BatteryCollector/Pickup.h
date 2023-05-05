// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h" // at the last of includes

UCLASS()
class BATTERYCOLLECTOR_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// isActive is True, this pickup can be used, otherwise is deactivated.
	bool bisActive;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Return the mesh for the pickup
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return PickupMesh;  }

	// return the state of pickup whether active or not
	UFUNCTION(BlueprintPure, Category="Pickup")
	bool IsActive();

	// safely change whether or not pickup is active.
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SetActive(bool NewPickupState);

	UFUNCTION(BlueprintNativeEvent)
	void WasCollected();
	virtual void WasCollected_Implementation();
private:
	// static mesh to represent the pickup in the level
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pickup", Meta=(AllowPrivateAccess="true"))
	class UStaticMeshComponent* PickupMesh;

};
