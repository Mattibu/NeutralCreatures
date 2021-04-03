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
	EUnitType GetUnitType();

protected:
	virtual void BeginPlay() override;

private:
	/** Set destination of controlled pawn */
	void SetMovementDestination(FVector DestLocation);
	/** Generate and set random movement location based on distance from properties */
	void GenerateNextMovementDestination();
	/** Do proper movement action */
	void Move();
	/** Calculate direction where controlled pawn should run away */
	FVector FindRunAwayDirection();
	/** Checs if distance between controlled pawn and other pawn is less or equal than distance */
	bool IsPawnTooClose(APawn* pawn, float distance);

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float m_MinDistanceX = -500;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float m_MaxDistanceX = 500;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float m_MinDistanceY = -500;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float m_MaxDistanceY = 500;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float m_MinWaitingTimeSeconds = 3;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float m_MaxWaitingTimeSeconds = 10;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float m_RunAwayDistance = 500;

	UPROPERTY(EditDefaultsOnly, Category = "UnitType")
	EUnitType m_UnitType;

	FTimerHandle m_WaitTimer;	
};
