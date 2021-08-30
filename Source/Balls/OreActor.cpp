// Fill out your copyright notice in the Description page of Project Settings.

#include "OreActor.h"
#include "Grid.h"
#include "PaperSpriteComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
AOreActor::AOreActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bCanEverTick = true; //Не нужно нам процессить кусок руды

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("OreBase"));
	}
	
	OreDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("OreDirection"));
	OreDirection->SetupAttachment(RootComponent);
	
	OreSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("OreSprite"));
	OreSprite->SetupAttachment(OreDirection);
	
}

// Called every frame
void AOreActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOreActor::SetOreType_Implementation(int32 NewType)
{
	if (CheckType(NewType))
	{
		OreType = NewType;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AOreActor try to set incorrect type: %d"), NewType);
	}
}

void AOreActor::SetOreStatus_Implementation(int32 NewStatus)
{
	if (NewStatus < 0 || NewStatus >= EOreStatus::EOS_NumberOfElements)
	{
		UE_LOG(LogTemp, Warning, TEXT("AOreActor try to set incorrect status: %d"), NewStatus);
	}
	else
	{
		OreStatus = NewStatus;
	}
}

bool  AOreActor::CheckType(int32 NewType)
{
	return (NewType > 0 && NewType < EOreType::EOT_NumberOfElements);
}
// Called when the game starts or when spawned
void AOreActor::BeginPlay()
{
	Super::BeginPlay();
	
}


void AOreActor::SetGridAddress(int32 NewAddres)
{
	GridAddress = NewAddres;
}

int32 AOreActor::GetGridAddress()
{
	return GridAddress;
}

void AOreActor::SetMasterGrid(AGrid* NewMaster)
{
	if (NewMaster)
	{
		MasterGrid = NewMaster;
	}
}

void AOreActor::OrePress(ETouchIndex::Type FingerIndex, AActor* TouchedActor)
{
	if (MasterGrid)
	{

		MasterGrid->SelectOre(this);
	}
}

void AOreActor::MoveToNewCell()
{
	//UE_LOG(LogTemp, Warning, TEXT("AOreActor Call MoveToNewCell_Implementation"));
	if (!OreDirection || !OreSprite)
		return;

	if (!MasterGrid)
		return;

	FVector NewLocation = MasterGrid->GetLocationFromGridAddress(GridAddress);

	this->SetActorLocation(NewLocation);
}


