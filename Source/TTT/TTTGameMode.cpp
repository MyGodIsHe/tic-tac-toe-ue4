// Fill out your copyright notice in the Description page of Project Settings.

#include "TTT.h"
#include "TTTPlayerController.h"
#include "TTTIronPlayer.h"
#include "TTTPawn.h"
#include "TTTGameMode.h"


ATTTGameMode::ATTTGameMode()
{
	PlayerControllerClass = ATTTPlayerController::StaticClass();
	DefaultPawnClass = ATTTPawn::StaticClass();

	
	WinSize = 3;
	FieldSize = 4;
}

void ATTTGameMode::BeginPlay()
{
	Super::BeginPlay();

	IsGameOver = false;

	ATTTPawn* Pawn = Cast<ATTTPawn>(*GetWorld()->GetPawnIterator());
	Pawn->Camera->SetWorldLocationAndRotation(FVector(0, 0, 1000.0f), FRotationMatrix::MakeFromX(FVector(0, 0, -1)).Rotator());

	GameField = GetWorld()->SpawnActor<AGameField>(FVector(0), FRotator(0, 0, 0), FActorSpawnParameters());
	GameField->Size = FieldSize;
	for (int32 i = 0; i < FieldSize; i++)
	{
		TArray<int32> SubArray;
		SubArray.Init(EMPTY_POSITION, FieldSize);
		FieldData.Add(SubArray);
	}

	Players.Add(Pawn);
	auto* AI = GetWorld()->SpawnActor<ATTTIronPlayer>(FVector(), FRotator());
	Players.Add(AI);
	CurrentPlayer = FMath::RandRange(0, Players.Num() - 1);

	for (int32 i = 0; i < Players.Num(); i++)
	{
		Players[i]->PlayerNumber = i;
		Players[i]->Sign = i == CurrentPlayer ? ESign::X : ESign::O;
	}
	Players[CurrentPlayer]->OnTurn();
}

void ATTTGameMode::SetCellSign(const int32 PlayerNumber, const FPosition& Position)
{
	if (IsGameOver || FieldData[Position.X][Position.Y] != EMPTY_POSITION || PlayerNumber != CurrentPlayer)
		return;
	FieldData[Position.X][Position.Y] = PlayerNumber;

	UClass* SignActor = Players[CurrentPlayer]->Sign == ESign::X ? SignXActor : SignOActor;
	FVector Location = GameField->GetActorLocation() + GameField->GetRelativeLocationByPosition(Position) + FVector(0, 0, 50);
	GetWorld()->SpawnActor(SignActor, &Location);

	if (IsWin(Position))
	{
		IsGameOver = true;
		Players[CurrentPlayer]->OnWin();
		for (int32 i = 0; i < Players.Num(); i++)
			if (i != CurrentPlayer)
				Players[i]->OnLose();
	}
	else
		TurnNextPlayer();
}

void ATTTGameMode::TurnNextPlayer()
{
	CurrentPlayer++;
	if (!Players.IsValidIndex(CurrentPlayer))
		CurrentPlayer = 0;
	Players[CurrentPlayer]->OnTurn();
}

bool ATTTGameMode::IsWin(const FPosition& Position)
{
	int32 offset = WinSize - 1;
	// horizontal lines
	for (int32 i = Position.X - offset; i <= Position.X; i++)
		if (IsWinLine(FPosition(i, Position.Y), FPosition(i + offset, Position.Y)))
			return true;

	// vertical lines
	for (int32 i = Position.Y - offset; i <= Position.Y; i++)
		if (IsWinLine(FPosition(Position.X, i), FPosition(Position.X, i + offset)))
			return true;

	// diagonal lines
	for (int32 i = -offset; i <= 0; i++)
	{
		if (IsWinLine(FPosition(Position.X + i, Position.Y + i), FPosition(Position.X + offset + i, Position.Y + offset + i)))
			return true;
		if (IsWinLine(FPosition(Position.X + i, Position.Y - i), FPosition(Position.X + offset + i, Position.Y - offset - i)))
			return true;
	}

	return false;
}

bool ATTTGameMode::IsFull()
{
	return false;
}

bool ATTTGameMode::AllEqual(const TArray<int32>& Array)
{
	if (Array.Num() == 0)
		return false;
	int32 Value = Array[0];
	if (Value == EMPTY_POSITION)
		return false;
	for (int32 i = 1; i < Array.Num(); i++)
		if (Value != Array[i])
			return false;
	return true;
};

TArray<int32> ATTTGameMode::GetLine(FPosition Begin, FPosition End)
{
	int32 xSign;
	if (Begin.X == End.X)
		xSign = 0;
	else
		xSign = Begin.X < End.X ? 1 : -1;

	int32 ySign;
	if (Begin.Y == End.Y)
		ySign = 0;
	else
		ySign = Begin.Y < End.Y ? 1 : -1;

	TArray<int32> line;
	int32 x = Begin.X - xSign, y = Begin.Y - ySign;
	do {
		x += xSign;
		y += ySign;
		line.Add(FieldData[x][y]);
	} while (x != End.X || y != End.Y);
	return line;
}

inline bool ATTTGameMode::IsValidPosition(FPosition Position)
{
	return 0 <= Position.X && Position.X < FieldSize && 0 <= Position.Y && Position.Y < FieldSize;
}

inline bool ATTTGameMode::IsWinLine(FPosition Begin, FPosition End)
{
	return IsValidPosition(Begin) && IsValidPosition(End) && AllEqual(GetLine(Begin, End));
}