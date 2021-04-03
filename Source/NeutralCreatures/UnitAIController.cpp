// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAIController.h"
#include "EngineUtils.h"
#include <NeutralCreatures\NeutralCreaturesCharacter.h>
#include <Runtime\AIModule\Classes\Blueprint\AIBlueprintHelperLibrary.h>
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>

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
	APawn* const controlled_pawn = GetPawn();
	if (controlled_pawn)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
	}
}

void AUnitAIController::GenerateNextMovementDestination()
{
	APawn* const controlled_pawn = GetPawn();
	if (!controlled_pawn)
	{
		return;
	}
	float distance_x = FMath::RandRange(m_RandMoveRangeMinX, m_RandMoveRangeMaxX);
	float distance_y = FMath::RandRange(m_RandMoveRangeMinY, m_RandMoveRangeMaxY);
	
	FVector location = controlled_pawn->GetActorLocation();

	SetMovementDestination(FVector(location.X + distance_x, location.Y + distance_y, location.Z));
}

void AUnitAIController::Move()
{
	APawn* const controlled_pawn = GetPawn();
	if (!controlled_pawn)
	{
		return;
	}

	if (m_UnitType == EUnitType::NEUTRAL)
	{
		FVector run_away_direction = FindRunAwayDirection();
		if (run_away_direction.Size() > 0)
		{
			//stop scheduled normal movement
			GetWorldTimerManager().ClearTimer(m_WaitTimer);
			SetMovementDestination(controlled_pawn->GetActorLocation() + run_away_direction * m_RunAwayMoveDistance);
			return;
		}
	}

	//when reach destination schedule next movement destination with random delay
	if (controlled_pawn->GetVelocity().Size() == 0.f && !GetWorldTimerManager().IsTimerActive(m_WaitTimer))
	{
		GetWorldTimerManager().SetTimer(m_WaitTimer, this, &AUnitAIController::GenerateNextMovementDestination,
										FMath::FRandRange(m_MinWaitingTimeSeconds, m_MaxWaitingTimeSeconds), false);
	}	
}

FVector AUnitAIController::FindRunAwayDirection() const
{
	APawn* controlled_pawn = GetPawn();

	FVector run_away_direction(0.f, 0.f, 0.f);

	if (!controlled_pawn)
	{
		return run_away_direction;
	}
	FVector my_pawn_location = controlled_pawn->GetActorLocation();

	for (AUnitAIController* Controller : TActorRange<AUnitAIController>(GetWorld()))
	{
		APawn* pawn = Controller->GetPawn();
		if (Controller->GetUnitType() == EUnitType::PLAYER_UNIT && pawn && IsPawnTooClose(pawn, m_RunAwayDetectDistance))
		{
			run_away_direction += my_pawn_location - pawn->GetActorLocation();
		}
	}
	run_away_direction.Normalize();
	return run_away_direction;
}

bool AUnitAIController::IsPawnTooClose(APawn* pawn, float distance) const
{
	APawn* controlled_pawn = GetPawn();

	if (!pawn || !controlled_pawn)
	{
		return false;
	}
	return FVector::Dist(controlled_pawn->GetActorLocation(), pawn->GetActorLocation()) <= distance;
}

EUnitType AUnitAIController::GetUnitType() const
{
	return m_UnitType;
}

