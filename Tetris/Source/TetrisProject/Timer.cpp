// Fill out your copyright notice in the Description page of Project Settings.


#include "Timer.h"

#include "Components/TextBlock.h"

void UTimer::NativeConstruct()
{
	Super::NativeConstruct();	
}

//Code Adapted from _benui, 2020
void UTimer::SetTimer(int32 newTime)
{
	timerLabel->SetText(FText::AsNumber(newTime));
}
//end of adapted code