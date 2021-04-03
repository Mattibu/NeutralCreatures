// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAIController.h"
#include <Runtime\AIModule\Classes\Blueprint\AIBlueprintHelperLibrary.h>
#include <NeutralCreatures\NeutralCreaturesCharacter.h>
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include "EngineUtils.h"

void AUnitAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AUnitAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Move();	
}

void AUnitAIController::SetMovementDestination(FVector DestLocation)
{
	APawn* const my_pawn = GetPawn();
	if (my_pawn)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
	}
}

void AUnitAIController::GenerateNextMovementDestination()
{
	APawn* const my_pawn = GetPawn();
	if (!my_pawn)
	{
		return;
	}
	float distance_x = FMath::RandRange(m_MinDistanceX, m_MaxDistanceX);
	float distance_y = FMath::RandRange(m_MinDistanceY, m_MaxDistanceY);
	
	FVector location = my_pawn->GetActorLocation();

	SetMovementDestination(FVector(location.X + distance_x, location.Y + distance_y, location.Z));
}

void AUnitAIController::Move()
{
	APawn* const my_pawn = GetPawn();
	if (!my_pawn)
	{
		return;
	}

	if (m_UnitType == EUnitType::NEUTRAL)
	{
		FVector run_away_direction = FindRunAwayDirection();
		if (run_away_direction.Size() > 0)
		{
			GetWorldTimerManager().ClearTimer(m_WaitTimer);
			SetMovementDestination(my_pawn->GetActorLocation() + run_away_direction * m_RunAwayDistance);
			return;
		}
	}

	if (my_pawn->GetVelocity().Size() == 0.f && !GetWorldTimerManager().IsTimerActive(m_WaitTimer))
	{
		GetWorldTimerManager().SetTimer(m_WaitTimer, this, &AUnitAIController::GenerateNextMovementDestination, 2, false);
	}	
}

FVector AUnitAIController::FindRunAwayDirection()
{
	APawn* my_pawn = GetPawn();

	FVector run_away_direction(0.f, 0.f, 0.f);

	if (!my_pawn)
	{
		return run_away_direction;
	}
	FVector my_pawn_location = my_pawn->GetActorLocation();

	for (AUnitAIController* Controller : TActorRange<AUnitAIController>(GetWorld()))
	{
		APawn* pawn = Controller->GetPawn();
		if (pawn  && Controller->GetUnitType() == EUnitType::PLAYER_UNIT && IsPawnTooClose(pawn, m_RunAwayDistance))
		{
			run_away_direction += my_pawn_location - pawn->GetActorLocation();
		}
	}
	run_away_direction.Normalize();
	return run_away_direction;
}

bool AUnitAIController::IsPawnTooClose(APawn* pawn, float distance)
{
	APawn* my_pawn = GetPawn();

	if (!pawn || !my_pawn)
	{
		return false;
	}
	return FVector::Dist(my_pawn->GetActorLocation(), pawn->GetActorLocation()) <= distance;
}

EUnitType AUnitAIController::GetUnitType()
{
	return m_UnitType;
}

