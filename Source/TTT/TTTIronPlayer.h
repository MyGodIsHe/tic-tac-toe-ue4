// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "TTTPlayerInterface.h"
#include "GameField.h"
#include "TTTIronPlayer.generated.h"


/**
 * 
 */
UCLASS()
class TTT_API ATTTIronPlayer : public APawn, public TTTPlayerInterface
{
	GENERATED_BODY()

public:

	float CalcualtePower(const FPosition Position);

	virtual void OnTurn() override;
	virtual void OnWin() override;
	virtual void OnLose() override;
};
