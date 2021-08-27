// Fill out your copyright notice in the Description page of Project Settings.

#include "OreActor.h"
#include "Grid.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AGrid::InitGrid()
{

	GameTiles.Empty(GridWidth * GridHeight);
	GameTiles.AddZeroed(GameTiles.Max());
	FVector SpawnLocation;
	for (int32 width = 0; width < GridWidth; width++)
	{
		for (int32 height = 0; height < GridHeight; height++)
		{
			int32 GridAddress;
			GetGridAddressWithOffset(0, height, width, GridAddress);
			//UE_LOG(LogTemp, Warning, TEXT("AGrid GetGridAddressWithOffset 0 %d %d %d"), width, height, GridAddress);
			SpawnLocation = GetLocationFromGridAddress(GridAddress);
			int32 RandomOreType = FMath::Rand();
			RandomOreType = RandomOreType % (EOreType::EOT_NumberOfElements);
			AddOreToGrid(SpawnLocation, GridAddress, RandomOreType);
		}
	}

}

AOreActor* AGrid::AddOreToGrid(FVector SpawnLocation, int32 SpawnGridAddress, int32 OreTypeID)
{
	// Check for a valid World:
	UWorld* const World = GetWorld();
	if (World)
	{
		// Set the spawn parameters.
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		// Tiles never rotate
		FRotator SpawnRotation(0.0f, 0.0f, 0.0f);
		TSubclassOf<class AOreActor> TileToSpawn;
		// Spawn the tile.
		AOreActor* NewOre = World->SpawnActor<AOreActor>(UsefulActorBP, SpawnLocation, SpawnRotation, SpawnParams);
		/*
		NewTile->GetRenderComponent()->SetMobility(EComponentMobility::Movable);
		NewTile->TileTypeID = TileTypeID;
		NewTile->Abilities = TileLibrary[TileTypeID].Abilities;
		NewTile->SetTileMaterial(TileMaterial);
		NewTile->SetGridAddress(SpawnGridAddress);
		*/
		GameTiles[SpawnGridAddress] = NewOre;
		NewOre->SetOreType(OreTypeID);
		NewOre->SetGridAddress(SpawnGridAddress);
		//UE_LOG(LogTemp, Warning, TEXT("AGrid Add new Ore at %d Type: %d"), SpawnGridAddress, OreTypeID);
		return NewOre;
		
	}
	return nullptr;
}

bool AGrid::GetGridAddressWithOffset(int32 InitialGridAddress, int32 XOffset, int32 YOffset, int32& ReturnGridAddress)
{
	int32 NewAxisValue;

	// Initialize to an invalid address.
	ReturnGridAddress = -1;

	// Check for going off the map in the X direction.
	check(GridWidth > 0);
	NewAxisValue = (InitialGridAddress % GridWidth) + XOffset;
	if (NewAxisValue != FMath::Clamp(NewAxisValue, 0, (GridWidth - 1)))
	{
		return false;
	}

	// Check for going off the map in the Y direction.
	NewAxisValue = (InitialGridAddress / GridWidth) + YOffset;
	if (NewAxisValue != FMath::Clamp(NewAxisValue, 0, (GridHeight - 1)))
	{
		return false;
	}

	ReturnGridAddress = (InitialGridAddress + XOffset + (YOffset * GridWidth));
	check(ReturnGridAddress >= 0);
	check(ReturnGridAddress < (GridWidth* GridHeight));
	return true;
}


FVector AGrid::GetLocationFromGridAddress(int32 GridAddress)
{
	FVector Center = GetActorLocation();
	FVector OutLocation = FVector(-(GridWidth * 0.5f) * TileSize.X + (TileSize.X * 0.5f), -(GridHeight * 0.5f) * TileSize.Y + (TileSize.Y * 0.5f), 0.0f);
	check(GridWidth > 0);
	OutLocation.X += TileSize.X * (float)(GridAddress % GridWidth);
	OutLocation.Y += TileSize.Y * (float)(GridAddress / GridWidth);
	OutLocation += Center;
	UE_LOG(LogTemp, Warning, TEXT("AGrid GetLocationFromGridAddress X %f Y %f"), OutLocation.X, OutLocation.Y);
	return OutLocation;
}

void AGrid::AfterPlayStart_Implementation()
{
	return;
}