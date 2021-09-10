// Fill out your copyright notice in the Description page of Project Settings.


#include "mach3GameMode.h"

void Amach3GameMode::BeginPlay()
{
	Super::BeginPlay();
	ResetScore();
}

int32 Amach3GameMode::GetScore()
{
	return Score;
}

void  Amach3GameMode::AddScore(int32 AdditionalScore)
{
	Score += AdditionalScore;
}

void Amach3GameMode::ResetScore()
{
	Score = 0;
}