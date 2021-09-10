// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "mach3GameMode.generated.h"

/**
 * 
 */
UCLASS()
class BALLS_API Amach3GameMode : public AGameModeBase
{
	GENERATED_BODY()
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetScore();
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 AdditionalScore);
	UFUNCTION(BlueprintCallable, Category = "Score")
	void ResetScore();
private:
	int32 Score;
};
