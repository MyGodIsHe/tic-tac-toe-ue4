// Fill out your copyright notice in the Description page of Project Settings.

#include "TTT.h"
#include "TTTGameMode.h"
#include "TTTIronPlayer.h"

void ATTTIronPlayer::OnTurn()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Iron Turn"));

	RunMinMax(PlayerNumber);
}

float ATTTIronPlayer::RunMinMax(const int32 CurrentPlayer, const int32 Level)
{
	ATTTGameMode* GameMode = (ATTTGameMode*)(GetWorld()->GetAuthGameMode());

	int32 NextPlayer = GameMode->GetNextPlayer(CurrentPlayer);
	bool IsMyTurn = (CurrentPlayer == PlayerNumber);
	TArray<FPosition> BestTurn;
	int32 MinMaxPower = IsMyTurn ? STATE_MIN : STATE_MAX;

	if (Level == DifficultyLevel * GameMode->Players.Num() + 1)
		return GameMode->FieldData->GetState(PlayerNumber);

	for (int32 x = 0; x < GameMode->FieldData->SizeX; x++)
		for (int32 y = 0; y < GameMode->FieldData->SizeY; y++)
			if (GameMode->FieldData->Get(x, y) == EMPTY_POSITION)
			{
				GameMode->FieldData->Set(x, y, CurrentPlayer);
				int32 Power = RunMinMax(NextPlayer, Level + 1);
				GameMode->FieldData->Set(x, y, EMPTY_POSITION);
				if (IsMyTurn && Power >= MinMaxPower || !IsMyTurn && Power <= MinMaxPower)
				{
					if (Power != MinMaxPower)
						BestTurn.Reset();
					BestTurn.Add(FPosition(x, y));
					MinMaxPower = Power;
				}
			}

	if (Level == 0)
	{
		GameMode->SetCellSign(PlayerNumber, BestTurn[FMath::RandRange(0, BestTurn.Num() - 1)]);
	}
	else
	{
		if (MinMaxPower == STATE_MIN || MinMaxPower == STATE_MAX)
			return GameMode->FieldData->GetState(PlayerNumber);
		else if (MinMaxPower == STATE_UNDEFINED)
			return GameMode->FieldData->GetState(PlayerNumber);
		else if (MinMaxPower == STATE_UNKNOWN)
			return STATE_UNKNOWN + GameMode->FieldData->GetHeuristicValue(PlayerNumber);
	}
	return MinMaxPower;
}

void ATTTIronPlayer::OnWin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Iron Win!"));
}

void ATTTIronPlayer::OnLose()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Iron Lose!"));
}