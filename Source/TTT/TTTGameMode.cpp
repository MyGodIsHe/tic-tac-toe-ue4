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

	
	WinSize = TField::WinSize;
	FieldSize = 3;
}

ATTTGameMode::~ATTTGameMode()
{
	delete FieldData;
}

void ATTTGameMode::BeginPlay()
{
	Super::BeginPlay();

	IsGameOver = false;

	ATTTPawn* Pawn = Cast<ATTTPawn>(*GetWorld()->GetPawnIterator());
	Pawn->Camera->SetWorldLocationAndRotation(FVector(0, 0, 1000.0f), FRotationMatrix::MakeFromX(FVector(0, 0, -1)).Rotator());

	GameField = GetWorld()->SpawnActor<AGameField>(FVector(0), FRotator(0, 0, 0), FActorSpawnParameters());
	GameField->Size = FieldSize;
	FieldData = new TField(FieldSize, FieldSize);
	TField::WinSize = WinSize;

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
	if (IsGameOver || FieldData->Get(Position) != EMPTY_POSITION || PlayerNumber != CurrentPlayer)
		return;
	FieldData->Set(Position, PlayerNumber);

	UClass* SignActor = Players[CurrentPlayer]->Sign == ESign::X ? SignXActor : SignOActor;
	FVector Location = GameField->GetActorLocation() + GameField->GetRelativeLocationByPosition(Position) + FVector(0, 0, 50);
	GetWorld()->SpawnActor(SignActor, &Location);

	if (FieldData->IsWinPosition(Position))
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

int32 ATTTGameMode::GetNextPlayer(int32 Player)
{
	Player++;
	if (!Players.IsValidIndex(Player))
		Player = 0;
	return Player;
}

void ATTTGameMode::TurnNextPlayer()
{
	CurrentPlayer = GetNextPlayer(CurrentPlayer);
	Players[CurrentPlayer]->OnTurn();
}