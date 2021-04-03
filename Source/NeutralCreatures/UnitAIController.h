// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UnitAIController.generated.h"


UENUM()
enum class EUnitType : uint8
{
	PLAYER_UNIT,
	NEUTRAL
};

UCLASS()
class NEUTRALCREATURES_API AUnitAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;
	EUnitType GetUnitType() const;

protected:
	virtual void BeginPlay() override;

private:
	/** Set movement destination of controlled pawn */
	void SetMovementDestination(FVector DestLocation);
	/** Generate and set random movement location based on the distance from properties */
	void GenerateNextMovementDestination();
	/** Do proper movement action */
	void Move();
	/** Calculate direction where controlled pawn should run away */
	FVector FindRunAwayDirection() const;
	/** Checks if distance between controlled pawn and other pawn is less or equal than the distance */
	bool IsPawnTooClose(APawn* pawn, float distance) const;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float m_RandMoveRangeMinX = -500;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float m_RandMoveRangeMaxX = 500;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float m_RandMoveRangeMinY = -500;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float m_RandMoveRangeMaxY = 500;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float m_MinWaitingTimeSeconds = 3;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float m_MaxWaitingTimeSeconds = 5;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float m_RunAwayDetectDistance = 500;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float m_RunAwayMoveDistance = 350;

	UPROPERTY(EditDefaultsOnly, Category = "UnitType")
	EUnitType m_UnitType;

	FTimerHandle m_WaitTimer;	
};
