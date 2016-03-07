// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

enum ESign { X, O };

/**
 * 
 */
class TTT_API TTTPlayerInterface
{
public:
	int32 PlayerNumber;
	ESign Sign;

	virtual void OnTurn() {};
	virtual void OnWin() {};
	virtual void OnLose() {};
};
