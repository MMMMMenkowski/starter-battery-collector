// Copyright Epic Games, Inc. All Rights Reserved.

#include "BatteryCollectorGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "BatteryCollectorCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SpawnVolume.h"

ABatteryCollectorGameMode::ABatteryCollectorGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	DecayRate = 0.3f;
}

void ABatteryCollectorGameMode::BeginPlay()
{
	Super::BeginPlay();

	// should be run before set the currentState to playing because the handler may need the spawnvolumeactors.
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);
	for (auto Actor : FoundActors)
	{
		ASpawnVolume* SpawnVolumeActor = Cast<ASpawnVolume>(Actor);
		if (SpawnVolumeActor)
		{
			SpawnVolumeActors.AddUnique(SpawnVolumeActor);
		}
	}

	SetCurrentState(EBatteryPlayState::Eplaying);
	ABatteryCollectorCharacter* MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter != nullptr)
	{
		PowerToWin = MyCharacter->GetInitailPower() * 1.2f;
	}

	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void ABatteryCollectorGameMode::Tick(float DeltaTime) // not called?
{
	//UE_LOG(LogClass, Log, TEXT("Tick function called."));
	Super::Tick(DeltaTime);
	ABatteryCollectorCharacter* MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	//if (UGameplayStatics::GetPlayerPawn(this, 0)) UE_LOG(LogClass, Log, TEXT("get the playerpawn successfully but wrong with cast to character."));
	if (MyCharacter != nullptr)
	{
		//UE_LOG(LogClass, Log, TEXT("Get PlayerPawn Successfully and ready to decay the power by time."));
		// when the current power of the player greater than power2win, the gameover to won, but the current power lower than 0, the game lost, these 2 situation dont need to decay the power of player.
		if (MyCharacter->GetCurrentPower() > PowerToWin)
		{
			SetCurrentState(EBatteryPlayState::EWon);
		}
		else if (MyCharacter->GetCurrentPower() > 0)
		{
			MyCharacter->ChangePower(-DeltaTime * DecayRate * MyCharacter->GetInitailPower());
		}
		else
		{
			SetCurrentState(EBatteryPlayState::EGameOver);
		}
	}
}

float ABatteryCollectorGameMode::GetPowerToWin() const
{
	return PowerToWin;
}

EBatteryPlayState ABatteryCollectorGameMode::GetCurrentState() const
{
	return CurrentState;
}

void ABatteryCollectorGameMode::SetCurrentState(EBatteryPlayState NewState)
{
	CurrentState = NewState;
	// each time set state function called, handle the new state changed.
	HandleNewState(NewState);
}

void ABatteryCollectorGameMode::HandleNewState(EBatteryPlayState State)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	switch (State)
	{
	case EBatteryPlayState::Eplaying:
		// make the spawnvolumes can spawn battries.
		for (auto SActor : SpawnVolumeActors)
		{
			SActor->SetState(true);
		}
		break;
	
	case EBatteryPlayState::EGameOver:
		for (auto SActor : SpawnVolumeActors)
		{
			SActor->SetState(false);
		}
		// block the input
		
		if (PlayerController)
		{
			PlayerController->SetCinematicMode(true, true, true);
		}
		// collapse
		
		if (MyCharacter)
		{
			MyCharacter->GetMesh()->SetSimulatePhysics(true);
			MyCharacter->GetMovementComponent()->MovementState.bCanJump = false;
		}
		break;

	case EBatteryPlayState::EWon:
		for (auto SActor : SpawnVolumeActors)
		{
			SActor->SetState(false);
		}
		
		break;

	
	case EBatteryPlayState::EUnknown:
	default:
		// doing nothing
		break;
	}
}