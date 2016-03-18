// Fill out your copyright notice in the Description page of Project Settings.

#include "TTT.h"
#include "TField.h"


TField::TField(const int32 InSizeX, const int32 InSizeY, const int32 InWinSize)
	: SizeX(InSizeX), SizeY(InSizeY), WinSize(InWinSize)
{
	check(InSizeX > 0 && InSizeY > 0 && InWinSize > 1);
	for (int32 i = 0; i < InSizeX; i++)
	{
		TArray<int32> SubArray;
		SubArray.Init(EMPTY_POSITION, InSizeY);
		Data.Add(SubArray);
	}

	for (int32 x = 0; x < SizeX; x++)
		for (int32 y = 0; y < SizeY; y++)
			for (auto& i : GetLinesByPosition(FPosition(x, y)))
				Lines.AddUnique(i);
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
	for (int32 x = 0; x < SizeX; x++)
		for (int32 y = 0; y < SizeY; y++)
			if (Data[x][y] == EMPTY_POSITION)
				return false;
	return true;
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


/*
return -1.0-1.0
*/
float TField::GetHeuristicValue(const int32 Player) const
{
	int32 Points = 0;
	for (auto& Line : Lines)
		if (IsValidPosition(Line.Begin) && IsValidPosition(Line.End))
		{
			TMap<int32, int32> Spectrum = GetSpectrum(Line.Begin, Line.End);
			if (Spectrum.Num() == 1)
				if (Spectrum.Contains(EMPTY_POSITION))
					Points += Spectrum.Contains(Player) ? 1 : -1;
			else if (Spectrum.Num() == 2)
			{
				if (Spectrum.Contains(EMPTY_POSITION)) // can win
				{
					int32 reps = 0;
					for (auto& p : Spectrum)
						if (p.Key != EMPTY_POSITION)
							reps = p.Value;
					check(reps > 0);
					Points += (1 + reps) * (Spectrum.Contains(Player) ? 1 : -1);
				}
			}
		}
	return (float)Points / Lines.Num() * WinSize;
}

float TField::GetState(const int32 Player) const
{
	int32 Wins = 0;
	int32 Losses = 0;
	bool IsFull = false;
	for (auto& Line : Lines)
		if (IsValidPosition(Line.Begin) && IsValidPosition(Line.End))
		{
			TMap<int32, int32> Spectrum = GetSpectrum(Line.Begin, Line.End);
			if (!IsFull && Spectrum.Contains(EMPTY_POSITION))
				IsFull = true;
			if (Spectrum.Num() == 1)
				if (Spectrum.Contains(Player))
					Wins++;
				else if (!Spectrum.Contains(EMPTY_POSITION))
					Losses++;
		}
	if (Wins && Losses)
		return STATE_UNDEFINED;
	if (Wins)
		return STATE_WIN;
	if (Losses)
		return STATE_LOSS;
	if (IsFull)
		return STATE_STANDOFF;
	return STATE_UNKNOWN;
}

TArray<FLine> TField::GetLinesByPosition(const FPosition Position) const
{
	int32 offset = WinSize - 1;
	TArray<FLine> Positions;

	// horizontal lines
	for (int32 i = Position.X - offset; i <= Position.X; i++)
		Positions.Add(FLine(FPosition(i, Position.Y), FPosition(i + offset, Position.Y)));

	// vertical lines
	for (int32 i = Position.Y - offset; i <= Position.Y; i++)
		Positions.Add(FLine(FPosition(Position.X, i), FPosition(Position.X, i + offset)));

	// diagonal lines
	for (int32 i = -offset; i <= 0; i++)
	{
		Positions.Add(FLine(FPosition(Position.X + i, Position.Y + i), FPosition(Position.X + offset + i, Position.Y + offset + i)));
		Positions.Add(FLine(FPosition(Position.X + i, Position.Y - i), FPosition(Position.X + offset + i, Position.Y - offset - i)));
	}
	return Positions;
}

/*
This is guaranteed to more than it can be.
Example for WinSize equal to 3:
x_x_x
_xxx_
xxxxx
_xxx_
x_x_x
*/
int32 TField::GetMaxCalculatedPoints() const
{
	return 4 * (1 + WinSize) * WinSize;
}

int32 TField::GetZugzwangPoints() const
{
	return GetMaxCalculatedPoints() + 1;
}

int32 TField::GetWinPoints() const
{
	return GetMaxCalculatedPoints() + 2;
}

int32 TField::GetBeforeLowPoints() const
{
	return -1;
}

int32 TField::GetAfterHighPoints() const
{
	return GetWinPoints() + 1;
}