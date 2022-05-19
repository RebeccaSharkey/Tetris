// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartMenu.generated.h"

UCLASS()
class TETRISPROJECT_API UStartMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;
	

public:

	UPROPERTY(meta = (BindWidget))
	class UButton* playButton;

};
