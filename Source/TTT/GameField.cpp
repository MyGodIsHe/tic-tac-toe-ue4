// Fill out your copyright notice in the Description page of Project Settings.

#include "TTT.h"
#include "GameField.h"

// Sets default values
AGameField::AGameField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Size = 3;
	CellPadding = 5;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultCellStaticMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	CellStaticMesh = DefaultCellStaticMesh.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial> DefaultCellMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	CellMaterial = DefaultCellMaterial.Object;
}

void AGameField::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!GetRootComponent()->AttachChildren.Num())
		GenerateField();
}

#if WITH_EDITOR
void AGameField::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	//Get the name of the property that was changed  
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AGameField, Size) || PropertyName == GET_MEMBER_NAME_CHECKED(AGameField, CellPadding))
	{
		GenerateField();
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AGameField, CellStaticMesh))
	{
		for (auto* c : GetRootComponent()->AttachChildren)
			if (UStaticMeshComponent* mc = Cast<UStaticMeshComponent>(c))
				mc->SetStaticMesh(CellStaticMesh);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AGameField, CellMaterial))
	{
		for (auto* c : GetRootComponent()->AttachChildren)
			if (UStaticMeshComponent* mc = Cast<UStaticMeshComponent>(c))
				mc->SetMaterial(0, CellMaterial);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AGameField, CellPadding))
	{
		for (auto* c : GetRootComponent()->AttachChildren)
			if (UStaticMeshComponent* mc = Cast<UStaticMeshComponent>(c))
				mc->SetMaterial(0, CellMaterial);
	}
}
#endif

// Called when the game starts or when spawned
void AGameField::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateField();
}

void AGameField::GenerateField()
{
	while (GetRootComponent()->AttachChildren.Num())
	{
		auto* c = GetRootComponent()->AttachChildren[GetRootComponent()->AttachChildren.Num() - 1];
		c->DestroyComponent();
	}
	Positions.Empty();

	for (int32 x = 0; x < Size; x++)
		for (int32 y = 0; y < Size; y++)
		{
			FName name = FName(*FString::Printf(TEXT("Cell[%d,%d]"), x, y));
			UStaticMeshComponent* mesh = NewObject<UStaticMeshComponent>(this, name);
			mesh->AttachTo(GetRootComponent());
			mesh->RegisterComponentWithWorld(GetWorld());
			mesh->SetStaticMesh(CellStaticMesh);
			mesh->SetMaterial(0, CellMaterial);
			mesh->SetRelativeLocation(GetRelativeLocationByPosition(FPosition(x, y)));
			Positions.Add(mesh, FPosition(x, y));
		}
}

FPosition AGameField::GetPosition(const FHitResult& Hit)
{
	return Positions[Hit.Component.Get()];
}

FVector AGameField::GetRelativeLocationByPosition(const FPosition& Position)
{
	return FVector(Position.X - (Size - 1) / 2.0, Position.Y - (Size - 1) / 2.0, 0) * (CellStaticMesh->ExtendedBounds.BoxExtent + CellPadding) * 2 - CellStaticMesh->ExtendedBounds.Origin;
}