// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// create the static mesh component
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	RootComponent = PickupMesh;
	// all pickup start with active. if want to set this to false until player do something, can create a class based on this or using blueprint to override it.
	bisActive = true;
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
/*
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

bool APickup::IsActive()
{
	return bisActive;
}

void APickup::SetActive(bool NewState)
{
	bisActive = NewState;
}
/*
void APickup::WasCollected()
{
	WasCollected_Implementation();
}
*/

void APickup::WasCollected_Implementation()
{
	FString PickupDebugString = GetName();
	UE_LOG(LogClass, Log, TEXT("You have collected %s"), *PickupDebugString);

}

