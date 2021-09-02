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
	PrimaryActorTick.bCanEverTick = true; 

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
	switch (OreStatus)
	{
		case(EOreStatus::EOS_Normal): return;
		case(EOreStatus::EOS_Moving): MoveToNewCell(DeltaTime);  break;
		case(EOreStatus::EOS_Choosen):	break;
		default: UE_LOG(LogTemp, Warning, TEXT("AOreActor have incorrect status: %d"), OreStatus);
	}
}

int32 AOreActor::GetOreType()
{
	return OreType;
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
	/*
	if (NewStatus < 0 || NewStatus >= EOreStatus::EOS_NumberOfElements)
	{
		UE_LOG(LogTemp, Warning, TEXT("AOreActor try to set incorrect status: %d"), NewStatus);
	}
	else
	{
		OreStatus = NewStatus;
	}
	*/
	UE_LOG(LogTemp, Warning, TEXT("AOreActor Call SetOreStatus_Implementation"));
	switch (NewStatus)
	{
		case(EOreStatus::EOS_Normal):	OreStatus = NewStatus; break;
		case(EOreStatus::EOS_Moving):	OreStatus = NewStatus; break;
		case(EOreStatus::EOS_Choosen):	OreStatus = NewStatus; break;
		default: UE_LOG(LogTemp, Warning, TEXT("AOreActor try to set incorrect status: %d"), NewStatus);
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

void AOreActor::MoveToNewCell(float DeltaTime, bool NeedTeleport)
{
	UE_LOG(LogTemp, Warning, TEXT("AOreActor Call MoveToNewCell"));
	if (!OreDirection || !OreSprite)
		return;

	if (!MasterGrid)
		return;
	

	FVector TargetLocation = MasterGrid->GetLocationFromGridAddress(GridAddress);
	FVector TargetDirection = TargetLocation - GetActorLocation();

	if (NeedTeleport || TargetDirection.IsNearlyZero() || TargetDirection.SizeSquared() <= FMath::Square((OreSpeed * DeltaTime)))
	{
		SetActorLocation(TargetLocation);
		SetOreStatus(EOreStatus::EOS_Normal);
		MasterGrid->CheckCombinationOre(GridAddress);
		return;
	}
		

	//получаем шажок на который нам надо сходить
	FVector OreStep = TargetDirection * OreSpeed * DeltaTime;
	FVector OreStepNormal = TargetDirection.GetSafeNormal() * OreSpeed * DeltaTime;

	UE_LOG(LogTemp, Warning, TEXT("AOreActor move tick: %f %f %f"), TargetDirection.X, TargetDirection.Y, TargetDirection.Z);
	//≈сли обычный вектор больше нормализованного
	if (OreStep.SizeSquared() > OreStepNormal.SizeSquared())
	{
		SetActorLocation(GetActorLocation() + OreStepNormal);
	}
	else
	{
		SetActorLocation(GetActorLocation() + OreStep);
	}
}


