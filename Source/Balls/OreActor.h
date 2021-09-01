// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OreActor.generated.h"


UENUM(BlueprintType)
namespace EOreStatus
{
	enum Type
	{
		EOS_Normal				UMETA(DisplayName = "Normal"),
		EOS_Falling				UMETA(DisplayName = "Falling"),
		EOS_Moving				UMETA(DisplayName = "Moving"),
		EOS_Choosen				UMETA(DisplayName = "Choosen"),
		EOS_PendingDelete		UMETA(DisplayName = "Pending Delete"),
		EOS_NumberOfElements	UMETA(Hidden)
	};
}

UENUM(BlueprintType)
namespace EOreType
{
	enum Type
	{
		EOT_Red 	UMETA(DisplayName = "Red"),
		EOT_Green	UMETA(DisplayName = "Green"),
		EOT_Cyan	UMETA(DisplayName = "Cyan"),
		EOT_NumberOfElements  UMETA( Hidden )
	};

}

class AGrid;

UCLASS()
class BALLS_API AOreActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOreActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Ore")
	int32 GetOreType();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Ore")
	void SetOreType(int32 NewType);
	void SetOreType_Implementation(int32 NewType);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Ore")
	void SetOreStatus(int32 NewStatus);
	void SetOreStatus_Implementation(int32 NewStatus);

	UFUNCTION(BlueprintCallable, Category = "Ore")
	bool CheckType(int32 NewType);

	void SetGridAddress(int32 NewAddres);

	int32 GetGridAddress();

	void SetMasterGrid(AGrid* NewMaster);

	//Когда кто-ткнул наш кусок руды
	UFUNCTION(BlueprintCallable, Category = "Ore")
	void OrePress(ETouchIndex::Type FingerIndex, AActor* TouchedActor);

	UFUNCTION(BlueprintCallable, Category = "Ore")
	void MoveToNewCell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(BlueprintReadOnly, Category = Tile)
	int32 GridAddress;

private:
	//Класс стерлочки - для отладки и привязки направления
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ore", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* OreDirection;

	//Спрайт руды
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ore", meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* OreSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ore", meta = (AllowPrivateAccess = "true"))
	int32 OreType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ore", meta = (AllowPrivateAccess = "true"))
	int32 OreStatus;

	AGrid* MasterGrid;
};
