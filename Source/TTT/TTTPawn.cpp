// Fill out your copyright notice in the Description page of Project Settings.

#include "TTT.h"
#include "GameField.h"
#include "TTTGameMode.h"
#include "TTTPawn.h"


// Sets default values
ATTTPawn::ATTTPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void ATTTPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATTTPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ATTTPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Click", IE_Pressed, this, &ATTTPawn::OnClick);
}

void ATTTPawn::OnClick()
{
	FHitResult Hit = FHitResult(ForceInit);
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	
	if (Hit.bBlockingHit)
	{
		if (AGameField* Field = Cast<AGameField>(Hit.GetActor()))
		{
			FPosition Position = Field->GetPosition(Hit);
			ATTTGameMode* GameMode = (ATTTGameMode*)(GetWorld()->GetAuthGameMode());
			GameMode->SetCellSign(PlayerNumber, Position);
		}
	}
}

void ATTTPawn::OnTurn()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Your Turn"));
}

void ATTTPawn::OnWin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Win!"));
}

void ATTTPawn::OnLose()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Lose!"));
}