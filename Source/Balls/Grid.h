// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

class AOreActor;
class Amach3GameMode;

UCLASS()
class BALLS_API AGrid : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrid();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Ore")
	bool GetGridAddressWithOffset(int32 InitialGridAddress, int32 XOffset, int32 YOffset, int32& ReturnGridAddress);

	UFUNCTION(BlueprintCallable, Category = "Ore")
	FVector GetLocationFromGridAddress(int32 GridAddress);

	//????????? ???? ??????????? ??????????? ???????
	UFUNCTION(BlueprintCallable, Category = "Initialization")
		void InitGrid();

	//?????? ????? ?????? ? ?????????? ????? ????
	UPROPERTY(EditAnywhere, Category = "Ore")
	FVector2D TileSize;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Grid")
	void AfterPlayStart();
	void AfterPlayStart_Implementation();

	UFUNCTION(BlueprintCallable, Category = "WorkWithOre")
	void SelectOre(AOreActor* NewSelectedOre);

	UFUNCTION(BlueprintCallable, Category = "WorkWithOre")
	void SwapOre(AOreActor* FirstSelectedOre, AOreActor* SecondSelectedOre);

	UFUNCTION(BlueprintCallable, Category = "WorkWithOre")
	bool CanSwapOre(AOreActor* FirstSelectedOre, AOreActor* SecondSelectedOre);

	UFUNCTION(BlueprintCallable, Category = "WorkWithOre")
	void RemoveOre(AOreActor* ChoosenOre);

	UFUNCTION(BlueprintCallable, Category = "WorkWithOre")
	void CheckCombinationOre(int32 ChekedOreAddress);

	UFUNCTION(BlueprintCallable, Category = "WorkWithOre")
	void ReCreateOre(AOreActor* ClicedOre);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "OreSoundsHelper")
	void PlayPickOreSounds();
	void PlayPickOreSounds_Implementation();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "OreSoundsHelper")
	void PlayDropOreSounds();
	void PlayDropOreSounds_Implementation();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "OreSoundsHelper")
	void PlayDestroyOreLineSounds();
	void PlayDestroyOreLineSounds_Implementation();

private:
	TArray <bool> NeedCheckColumn;
	TArray<AOreActor*> GameTiles;

	UPROPERTY(EditAnywhere, Category = "Tile")
		int32 GridWidth;
	UPROPERTY(EditAnywhere, Category = "Tile")
		int32 GridHeight;


	AOreActor* AddOreToGrid(FVector SpawnLocation, int32 SpawnGridAddress, int32 TileTypeID);


	//?????? ??? ?????? ??????? ????? ????????
	UPROPERTY(EditDefaultsOnly, Category = "OreSpawning")
	TSubclassOf<AOreActor> UsefulActorBP;

	AOreActor* SelectedOre;

	bool CheckNeighbourOre(TArray<int32>& OresArray, int32& CenterOre, int32 OffsetStepX, int32 OffsetStepY);

	void CheckGridColumn();

	void RotateOreColumn(int32 TargetAddress);

	Amach3GameMode* GameMode;

};
