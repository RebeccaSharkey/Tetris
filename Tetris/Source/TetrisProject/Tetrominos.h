// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Cell.h"
#include "GameFramework/Actor.h"
#include "Tetrominos.generated.h"

UCLASS()
class TETRISPROJECT_API ATetrominos : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATetrominos();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void SetUpTetromino(ACell* (*gridPostions)[4][4], bool state, UMaterialInterface* current);
	
	void SetPostion(ACell* (*gridPostions)[4][4]);
	ACell* GetPosition();
	void DeleteOldPosition();
	
	int CheckForShape(int x, int y);
		
	int tempShape[4][4];
	void RotateShape(int direction);
	void SetRotation();
	
	void RemoveCurrentTetrominoMarker();

	UMaterialInterface* GetCurrentMaterial();
	
	ACell* currentPostion[4][4];
private:
	int shape[4][4];
	void SetShape();
	
	bool isCurrentTetromino;
	

	UMaterialInterface* currentMaterial;
};
