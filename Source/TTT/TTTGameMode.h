// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "TTTPlayerInterface.h"
#include "GameField.h"
#include "TTTGameMode.generated.h"


const int32 EMPTY_POSITION = -1;

/**
 * 
 */
UCLASS()
class TTT_API ATTTGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	bool IsGameOver;
	int32 FieldSize;
	TArray<TTTPlayerInterface*> Players;
	int32 CurrentPlayer;
	AGameField* GameField;
	TArray<TArray<int32>> FieldData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 WinSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AActor> SignXActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AActor> SignOActor;

	ATTTGameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetCellSign(const int32 PlayerNumber, const FPosition& Position);

	void TurnNextPlayer();

	bool IsWin(const FPosition& Position);

	bool IsFull();

	bool AllEqual(const TArray<int32>& Array);

	TArray<int32> GetLine(FPosition Begin, FPosition End);

	inline bool IsValidPosition(FPosition Position);

	inline bool IsWinLine(FPosition Begin, FPosition End);
};
