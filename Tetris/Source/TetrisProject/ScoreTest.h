// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreTest.generated.h"

UCLASS()
class TETRISPROJECT_API UScoreTest : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* scoreLabel;
	
	void NativeConstruct() override;

public:
	void SetScore(int64 newScore);
};
