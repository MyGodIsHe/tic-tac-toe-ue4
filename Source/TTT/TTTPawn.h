// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "TTTPlayerInterface.h"
#include "TTTPawn.generated.h"

UCLASS()
class TTT_API ATTTPawn : public APawn, public TTTPlayerInterface
{
	GENERATED_BODY()

public:

	UCameraComponent* Camera;

	// Sets default values for this pawn's properties
	ATTTPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void OnTurn() override;
	virtual void OnWin() override;
	virtual void OnLose() override;

	UFUNCTION()
	void OnClick();
	
};
