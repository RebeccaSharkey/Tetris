// Fill out your copyright notice in the Description page of Project Settings.

#include "ScoreTest.h"
#include "Components/TextBlock.h"

void UScoreTest::NativeConstruct()
{
	Super::NativeConstruct();	
}

//Code Adapted from _benui, 2020
void UScoreTest::SetScore(int64 newScore)
{
	scoreLabel->SetText(FText::AsNumber(newScore));
}
//end of adapted code