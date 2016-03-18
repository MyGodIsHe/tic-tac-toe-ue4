// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GameField.generated.h"


struct FPosition
{
	int32 X;
	int32 Y;

	FPosition()
	{
		X = 0;
		Y = 0;
	}

	FPosition(int32 InXY)
	{
		X = InXY;
		Y = InXY;
	}

	FPosition(int32 InX, int32 InY)
	{
		X = InX;
		Y = InY;
	}

	const bool operator==(const FPosition& rv) const {
		return X == rv.X && Y == rv.Y;
	}
};


UCLASS()
class TTT_API AGameField : public AActor
{
	GENERATED_BODY()
	
public:

	TMap<UPrimitiveComponent*, FPosition> Positions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Size;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* CellStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterial* CellMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CellPadding;

	// Sets default values for this actor's properties
	AGameField();

	virtual void OnConstruction(const FTransform& Transform) override;

#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent);
#endif

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void GenerateField();

	FPosition GetPosition(const FHitResult& Hit);

	FVector GetRelativeLocationByPosition(const FPosition& Position);
	
};
