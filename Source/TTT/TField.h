// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameField.h"


const int32 EMPTY_POSITION = -1;

const float STATE_MIN = -1;
const float STATE_UNDEFINED = 0;
const float STATE_LOSS = 1;
const float STATE_STANDOFF = 2;
const float STATE_UNKNOWN = 3;
const float STATE_WIN = 4;
const float STATE_MAX = 5;

struct FLine
{
	FPosition Begin;
	FPosition End;

	FLine(const FPosition InBegin, const FPosition InEnd)
	{
		Begin = InBegin;
		End = InEnd;
	}

	const bool operator==(const FLine& rv) const {
		return Begin == rv.Begin && End == rv.End;
	}
};


/**
 * 
 */
class TTT_API TField
{
private:
	TArray<TArray<int32>> Data;

public:

	const int32 WinSize;
	const int32 SizeX;
	const int32 SizeY;
	TArray<FLine> Lines;

	TField(const int32 InSizeX, const int32 InSizeY, const int32 InWinSize);

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

	float GetHeuristicValue(const int32 Player) const;

	float GetState(const int32 Player) const;

	TArray<FLine> GetLinesByPosition(const FPosition Position) const;

	int32 GetMaxCalculatedPoints() const;

	int32 GetZugzwangPoints() const;

	int32 GetWinPoints() const;

	int32 GetBeforeLowPoints() const;

	int32 GetAfterHighPoints() const;
};
