// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Timer.generated.h"

UCLASS()
class TETRISPROJECT_API UTimer : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* timerLabel;
	
	void NativeConstruct() override;

public:
	void SetTimer(int32 newTime);
};
