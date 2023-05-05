// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryPickup.h"

// Set default values
ABatteryPickup::ABatteryPickup()
{
	GetMesh()->SetSimulatePhysics(true);
	BatteryPower = 200.0f;
}

void ABatteryPickup::WasCollected_Implementation()
{
	// use the base pickup behavior first.
	Super::WasCollected_Implementation();
	// destroy the battery;
	Destroy();
}

// check the sequence between wascollected function and getpower and the latter must be behind the former.
float ABatteryPickup::GetPower()
{
	return BatteryPower;
}
