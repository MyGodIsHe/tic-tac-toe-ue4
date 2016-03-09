// Fill out your copyright notice in the Description page of Project Settings.

#include "TTT.h"
#include "TField.h"


int32 TField::WinSize = 3;

TField::TField(const int32 InSizeX, const int32 InSizeY)
	: SizeX(InSizeX), SizeY(InSizeY)
{
	check(InSizeX > 0 && InSizeY > 0);
	for (int32 i = 0; i < InSizeX; i++)
	{
		TArray<int32> SubArray;
		SubArray.Init(EMPTY_POSITION, InSizeY);
		Data.Add(SubArray);
	}
}

int32 TField::Get(const int32 X, const int32 Y) const
{
	return Data[X][Y];
}

int32 TField::Get(const FPosition Position) const
{
	return Data[Position.X][Position.Y];
}

void TField::Set(const int32 X, const int32 Y, const int32 Value)
{
	Data[X][Y] = Value;
}

void TField::Set(const FPosition Position, const int32 Value)
{
	Data[Position.X][Position.Y] = Value;
}

bool TField::IsWinPosition(const FPosition Position) const
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

bool TField::IsFull() const
{
	return false;
}

bool TField::AllEqual(const TArray<int32>& Array) const
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

TArray<int32> TField::GetLine(const FPosition Begin, const FPosition End) const
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
		line.Add(Data[x][y]);
	} while (x != End.X || y != End.Y);
	return line;
}

inline bool TField::IsValidPosition(const FPosition Position) const
{
	return 0 <= Position.X && Position.X < SizeX && 0 <= Position.Y && Position.Y < SizeY;
}

inline bool TField::IsWinLine(const FPosition Begin, const FPosition End) const
{
	return IsValidPosition(Begin) && IsValidPosition(End) && AllEqual(GetLine(Begin, End));
}

TMap<int32, int32> TField::GetSpectrum(const FPosition Begin, const FPosition End) const
{
	TMap<int32, int32> Spectrum;
	TArray<int32> Line = GetLine(Begin, End);
	for (int32 i = 0; i < Line.Num(); i++)
		if (Spectrum.Contains(Line[i]))
			Spectrum[Line[i]]++;
		else
			Spectrum.Add(Line[i], 1);
	return Spectrum;
}

int32 TField::GetHeuristicValue(const FPosition Position) const
{
	int32 offset = WinSize - 1;
	TArray<FPosition> Positions;

	// horizontal lines
	for (int32 i = Position.X - offset; i <= Position.X; i++)
	{
		Positions.Add(FPosition(i, Position.Y));
		Positions.Add(FPosition(i + offset, Position.Y));
	}

	// vertical lines
	for (int32 i = Position.Y - offset; i <= Position.Y; i++)
	{
		Positions.Add(FPosition(Position.X, i));
		Positions.Add(FPosition(Position.X, i + offset));
	}

	// diagonal lines
	for (int32 i = -offset; i <= 0; i++)
	{
		Positions.Add(FPosition(Position.X + i, Position.Y + i));
		Positions.Add(FPosition(Position.X + offset + i, Position.Y + offset + i));
		Positions.Add(FPosition(Position.X + i, Position.Y - i));
		Positions.Add(FPosition(Position.X + offset + i, Position.Y - offset - i));
	}

	int32 Points = 0;
	int32 Player = Get(Position);
	for (int32 i = 0; i < Positions.Num(); i+=2)
		if (IsValidPosition(Positions[i]) && IsValidPosition(Positions[i + 1]))
		{
			TMap<int32, int32> Spectrum = GetSpectrum(Positions[i], Positions[i + 1]);
			if (Spectrum.Num() == 1) // is win
				return GetMaxPoints();
			if (Spectrum.Num() == 2)
			{
				if (Spectrum.Contains(EMPTY_POSITION)) // can win
					Points += Spectrum[Player];
				else if (Spectrum[Player] == 1) // can lose
					return GetMaxPoints() - 1;
			}
		}
	return Points;
}

int32 TField::GetMaxPoints()
{
	return 4 * WinSize * WinSize;
}