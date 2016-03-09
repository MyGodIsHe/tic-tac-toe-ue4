// Fill out your copyright notice in the Description page of Project Settings.

#include "TTT.h"
#include "TTTGameMode.h"
#include "TTTIronPlayer.h"


void ATTTIronPlayer::OnTurn()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Iron Turn"));

	ATTTGameMode* GameMode = (ATTTGameMode*)(GetWorld()->GetAuthGameMode());
	FPosition BestTurn;
	float TurnPower = -TField::GetMaxPoints() - 1;
	for (int32 x = 0; x < GameMode->FieldData->SizeX; x++)
		for (int32 y = 0; y < GameMode->FieldData->SizeY; y++)
			if (GameMode->FieldData->Get(x, y) == EMPTY_POSITION)
			{
				GameMode->FieldData->Set(x, y, PlayerNumber);
				float Power = GameMode->FieldData->GetHeuristicValue(FPosition(x, y));
				GameMode->FieldData->Set(x, y, EMPTY_POSITION);
				if (Power >= TurnPower)
				{
					BestTurn = FPosition(x, y);
					TurnPower = Power;
				}
			}
	GameMode->SetCellSign(PlayerNumber, BestTurn);
}

void ATTTIronPlayer::OnWin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Iron Win!"));
}

void ATTTIronPlayer::OnLose()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Iron Lose!"));
}