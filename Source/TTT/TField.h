// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameField.h"


const int32 EMPTY_POSITION = -1;

/**
 * 
 */
class TTT_API TField
{
private:
	TArray<TArray<int32>> Data;

public:

	static int32 WinSize;
	const int32 SizeX;
	const int32 SizeY;

	TField(const int32 InSizeX, const int32 InSizeY);

	int32 Get(const int32 X, const int32 Y) const;
	int32 Get(const FPosition Position) const;

	void Set(const int32 X, const int32 Y, const int32 Value);
	void Set(const FPosition Position, const int32 Value);

	bool IsWinPosition(const FPosition Position) const;

	bool IsFull() const;

	bool AllEqual(const TArray<int32>& Array) const;

	TArray<int32> GetLine(const FPosition Begin, const FPosition End) const;

	inline bool IsValidPosition(const FPosition Position) const;

	inline bool IsWinLine(const FPosition Begin, const FPosition End) const;

	TMap<int32, int32> GetSpectrum(const FPosition Begin, const FPosition End) const;

	int32 GetHeuristicValue(const FPosition Position) const;

	static int32 GetMaxPoints();
};
