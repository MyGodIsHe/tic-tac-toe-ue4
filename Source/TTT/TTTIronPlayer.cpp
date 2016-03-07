// Fill out your copyright notice in the Description page of Project Settings.

#include "TTT.h"
#include "TTTGameMode.h"
#include "TTTIronPlayer.h"


void ATTTIronPlayer::OnTurn()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Iron Turn"));

	ATTTGameMode* GameMode = (ATTTGameMode*)(GetWorld()->GetAuthGameMode());
	FPosition BestTurn;
	float TurnPower = 0;
	for (int32 x = 0; x < GameMode->FieldData.Num(); x++)
		for (int32 y = 0; y < GameMode->FieldData[x].Num(); y++)
			if (GameMode->FieldData[x][y] == EMPTY_POSITION)
			{
				float Power = CalcualtePower(FPosition(x, y));
				if (Power >= TurnPower)
				{
					BestTurn = FPosition(x, y);
					TurnPower = Power;
				}
			}
	GameMode->SetCellSign(PlayerNumber, BestTurn);
}

float ATTTIronPlayer::CalcualtePower(const FPosition Position)
{

	return 0;
}

void ATTTIronPlayer::OnWin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Iron Win!"));
}

void ATTTIronPlayer::OnLose()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Iron Lose!"));
}