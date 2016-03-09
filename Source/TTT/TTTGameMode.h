// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "TTTPlayerInterface.h"
#include "TField.h"
#include "TTTGameMode.generated.h"


/**
 * 
 */
UCLASS()
class TTT_API ATTTGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	bool IsGameOver;
	TArray<TTTPlayerInterface*> Players;
	int32 CurrentPlayer;
	AGameField* GameField;
	TField* FieldData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 FieldSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 WinSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AActor> SignXActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AActor> SignOActor;

	ATTTGameMode();

	~ATTTGameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetCellSign(const int32 PlayerNumber, const FPosition& Position);

	int32 GetNextPlayer(int32 Player);

	void TurnNextPlayer();
};
