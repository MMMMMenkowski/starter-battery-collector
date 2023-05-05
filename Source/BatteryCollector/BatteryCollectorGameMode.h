// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Class.h"
#include "BatteryCollectorGameMode.generated.h"

UENUM(BlueprintType)
enum class EBatteryPlayState : uint8
{
	Eplaying,
	EGameOver,
	EWon,
	EUnknown
};

UCLASS(minimalapi)
class ABatteryCollectorGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ABatteryCollectorGameMode();

	// the PowerToWin should be set here because there must assure the Character have been created when set power2win which is relative.
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	// interface to get the power2win for HUD and blueprint
	UFUNCTION(BlueprintPure, Category="Power")
	float GetPowerToWin() const;

	// getter and setter for player state
	UFUNCTION(BlueprintPure, Category="Power")
	EBatteryPlayState GetCurrentState() const;

	void SetCurrentState(EBatteryPlayState NewState);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Power", Meta=(BlueprintProtected="true"))
	float DecayRate;

	// set the energy totally needed for win
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	float PowerToWin;

	// the UMG Widget HUD onscreen
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY()
	class UUserWidget* CurrentWidget; // the widget which spawned really.

private:
	// the class enum for sign the player state.
	EBatteryPlayState CurrentState;
	
	TArray<class ASpawnVolume*> SpawnVolumeActors;

	// handle each playState
	void HandleNewState(EBatteryPlayState State);
};






