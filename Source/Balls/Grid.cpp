// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"
#include "OreActor.h"
#include "mach3GameMode.h"


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
	GameMode = GetWorld()->GetAuthGameMode<Amach3GameMode>();
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckGridColumn();
}


void AGrid::InitGrid()
{
	NeedCheckColumn.Init(true, GridWidth);
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

		GameTiles[SpawnGridAddress] = NewOre;
		NewOre->SetOreType(OreTypeID);
		NewOre->SetGridAddress(SpawnGridAddress);
		NewOre->SetMasterGrid(this);
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

void AGrid::SelectOre(AOreActor* NewSelectedOre)
{
	
	//UE_LOG(LogTemp, Warning, TEXT("AGrid NewSelectedOre if %s"), NewSelectedOre ? TEXT("true") : TEXT("false"));
	//UE_LOG(LogTemp, Warning, TEXT("AGrid SelectedOre if %s"), SelectedOre ? TEXT("true") : TEXT("false"));
	if (!NewSelectedOre)
	{
		return;
	}

	if (NewSelectedOre == SelectedOre)
	{
		SelectedOre->SetOreStatus(EOreStatus::EOS_Normal);
		SelectedOre = nullptr;
		return;
	}

	if (NewSelectedOre != SelectedOre && SelectedOre != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("AGrid Call Swap"));
		NewSelectedOre->SetOreStatus(EOreStatus::EOS_Moving);
		SelectedOre->SetOreStatus(EOreStatus::EOS_Moving);
		SwapOre(NewSelectedOre, SelectedOre);
		SelectedOre = nullptr;
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("AGrid Call SetOreStatus"));
	SelectedOre = NewSelectedOre;
	SelectedOre->SetOreStatus(EOreStatus::EOS_Choosen);
}

void AGrid::SwapOre(AOreActor* FirstSelectedOre, AOreActor* SecondSelectedOre)
{
	
	int32 FirstAddres = FirstSelectedOre->GetGridAddress();
	int32 SecondAddres = SecondSelectedOre->GetGridAddress();

	FirstSelectedOre->SetGridAddress(SecondAddres);
	SecondSelectedOre->SetGridAddress(FirstAddres);

	GameTiles[FirstAddres] = SecondSelectedOre;
	GameTiles[SecondAddres] = FirstSelectedOre;

	//FirstSelectedOre->SetOreStatus(EOreStatus::EOS_Moving);
	//SecondSelectedOre->SetOreStatus(EOreStatus::EOS_Moving);

	SelectedOre = nullptr;

}


void AGrid::RemoveOre(AOreActor* ChoosenOre)
{
	//GameTiles[SelectedOre->GetGridAddress()] = nullptr;
	//SelectedOre->ClearOre();
	return;
}

FVector AGrid::GetLocationFromGridAddress(int32 GridAddress)
{
	FVector Center = GetActorLocation();
	FVector OutLocation = FVector(-(GridWidth * 0.5f) * TileSize.X + (TileSize.X * 0.5f), -(GridHeight * 0.5f) * TileSize.Y + (TileSize.Y * 0.5f), 0.0f);
	check(GridWidth > 0);
	OutLocation.X += TileSize.X * (float)(GridAddress % GridWidth);
	OutLocation.Y += TileSize.Y * (float)(GridAddress / GridWidth);
	OutLocation += Center;
	//UE_LOG(LogTemp, Warning, TEXT("AGrid GetLocationFromGridAddress X %f Y %f"), OutLocation.X, OutLocation.Y);
	return OutLocation;
}

void AGrid::AfterPlayStart_Implementation()
{
	return;
}

void AGrid::CheckCombinationOre(int32 ChekedOreAddress)
{
	UE_LOG(LogTemp, Warning, TEXT("AGrid Call CheckCombinationOre address:: %d"), ChekedOreAddress);
	TArray<int32> RelativesOreAddress;
	RelativesOreAddress.Add(ChekedOreAddress);
	//Проверяем соседние тайлы по горизонтали

	int32 WalkingOre = ChekedOreAddress;

	bool CanCheck = true;
	CanCheck = CheckNeighbourOre(RelativesOreAddress, ChekedOreAddress, 1, 0);
	WalkingOre = ChekedOreAddress;
	CanCheck = CanCheck && CheckNeighbourOre(RelativesOreAddress, ChekedOreAddress, -1, 0);

	if (!CanCheck) //Если при проверке встречаем двигающиеся куски руды, подходящие нам то они сами вызовут проверку.
	{
		return;
	}
	if(RelativesOreAddress.Num() >= 3)
	{ 
		for (auto RelativesOre : RelativesOreAddress)
		{
			NeedCheckColumn[RelativesOre % GridWidth] = true;
			GameTiles[RelativesOre]->SetOreStatus(EOreStatus::EOS_PendingDelete);
			
		}
		GameMode->AddScore(RelativesOreAddress.Num() * 10);
	}

	RelativesOreAddress.Empty();

	RelativesOreAddress.Add(ChekedOreAddress);
	WalkingOre = ChekedOreAddress;
	CanCheck = CheckNeighbourOre(RelativesOreAddress, ChekedOreAddress, 0, 1);
	WalkingOre = ChekedOreAddress;
	CanCheck = CanCheck && CheckNeighbourOre(RelativesOreAddress, ChekedOreAddress, 0, -1);

	if (!CanCheck) //Если при проверке встречаем двигающиеся куски руды, подходящие нам то они сами вызовут проверку.
	{
		return;
	}
	if (RelativesOreAddress.Num() >= 3)
	{
		for (auto RelativesOre : RelativesOreAddress)
		{
			NeedCheckColumn[RelativesOre % GridWidth] = true;
			GameTiles[RelativesOre]->SetOreStatus(EOreStatus::EOS_PendingDelete);
		}
		GameMode->AddScore(RelativesOreAddress.Num() * 10);
	}
}

bool AGrid::CheckNeighbourOre(TArray<int32>& OresArray, int32& CenterOre, int32 OffsetStepX, int32 OffsetStepY)
{
	int32 WalkingOre = CenterOre;
	int32 NeighbourOre;
	while (true)
	{
		if (!GetGridAddressWithOffset(WalkingOre, OffsetStepX, OffsetStepY, NeighbourOre))
			break;

		if (GameTiles[WalkingOre]->GetOreType() != GameTiles[NeighbourOre]->GetOreType())
			break;

		if (GameTiles[NeighbourOre]->GetOreStatus() == EOreStatus::EOS_Moving) // Если есть ещё дигающиеся тайлы, то после остановки они вызовут проверку.
		{
			return false;
		}
		OresArray.Add(NeighbourOre);
		WalkingOre = NeighbourOre;
	}

	return true;
}

void AGrid::CheckGridColumn()
{
	for (int Column = 0; Column < NeedCheckColumn.Num(); Column++)
	{
		if (NeedCheckColumn[Column])
		{	/*
			UE_LOG(LogTemp, Warning, TEXT("AGrid CheckGridColumn start check Column %d"), Column);
			for (int row = 0; row < GridHeight; row++)
			{
				int32 OreAddres = Column + row * GridWidth;
				int32 OreStatus = GameTiles[OreAddres]->GetOreStatus();
				
				if (OreStatus == EOreStatus::EOS_Normal )
				{
					continue;
				}
				if (OreStatus == EOreStatus::EOS_PendingDelete)
				{
					bool needBreak = false;
					//Пытаемся найти кусок руды с которым можно обменятся.
					for (int i = row + 1; i < GridHeight; i++)
					{
						int32 OreSwapAddres = Column + i * GridWidth;
						int32 OreSwapStatus = GameTiles[OreSwapAddres]->GetOreStatus();
						if (OreSwapStatus != EOreStatus::EOS_Normal)
						{
							continue;
						}
						SwapOre(GameTiles[OreAddres], GameTiles[OreSwapAddres]);
						break;
					}

					ReCreateOre(GameTiles[OreAddres]);
				}
				*/
				
			for (int Row = 0; Row < GridHeight; Row++)
			{
				int32 OreAddres;
				GetGridAddressWithOffset(0, Column, Row, OreAddres);
				int32 OreStatus = GameTiles[OreAddres]->GetOreStatus();

				if (OreStatus == EOreStatus::EOS_Normal)
				{
					continue;
				}
				if (OreStatus == EOreStatus::EOS_PendingDelete)
				{
					int32 SwapAddress;
					bool FindSwapOre = false; 
					for (int32 i = Row + 1; i < GridHeight; i++)
					{
						if (GetGridAddressWithOffset(0, Column, i, SwapAddress))
						{
							if (GameTiles[SwapAddress]->GetOreStatus() == EOreStatus::EOS_Normal)
							{
								SwapOre(GameTiles[OreAddres], GameTiles[SwapAddress]);
								GameTiles[OreAddres]->SetOreStatus(EOreStatus::EOS_Moving);
								FindSwapOre = true;
								break;
							}		
						}
					}
					//Если не смогли свапнуться.
					if (!FindSwapOre)
					{
						for (int32 i = Row; i < GridHeight; i++)
						{
							if (GetGridAddressWithOffset(0, Column, i, SwapAddress))
							{
								ReCreateOre(GameTiles[SwapAddress]);
							}
						}
					}
				}
			}
			NeedCheckColumn[Column] = false;
		}
	}
}

void AGrid::ReCreateOre(AOreActor* ClicedOre)
{
	int32 SpawnGridAddress = ClicedOre->GetGridAddress();
	//UE_LOG(LogTemp, Warning, TEXT("AGrid GetGridAddressWithOffset 0 %d %d %d"), width, height, GridAddress);
	FVector SpawnLocation = GetLocationFromGridAddress(SpawnGridAddress + GridHeight * GridWidth);
	int32 RandomOreType = FMath::Rand();
	RandomOreType = RandomOreType % (EOreType::EOT_NumberOfElements);
	GameTiles[SpawnGridAddress]->SetOreStatus(EOreStatus::EOS_Normal); //Для сброоса увеличения или включения показа спрайта
	GameTiles[SpawnGridAddress]->SetOreType(RandomOreType);
	GameTiles[SpawnGridAddress]->ForseMoveToLocation(SpawnLocation);
	//Отправляем кусок руды в полёт
	GameTiles[SpawnGridAddress]->SetOreStatus(EOreStatus::EOS_Moving);
}

void AGrid::RotateOreColumn(int32 TargetAddress)
{
}