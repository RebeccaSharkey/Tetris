// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndScreen.generated.h"

UCLASS()
class TETRISPROJECT_API UEndScreen : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

public:

	UPROPERTY(meta = (BindWidget))
	class UButton* restartButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* playerOnescoreLabel;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* playerTwoscoreLabel;
};
