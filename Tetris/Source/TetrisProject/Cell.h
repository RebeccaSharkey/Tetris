// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Cell.generated.h"

UCLASS()
class TETRISPROJECT_API ACell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACell();

private:
	bool isOccupied;
	bool isCurrentTetromino;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* cube;
	void SetMesh();
	UMaterialInstanceDynamic* dynamicMaterial;
	UMaterialInterface* currentMaterial;
	
public:
	void SetOccupiedStatus(bool newState, bool currentTetromino, UMaterialInterface* newMaterial);
	bool GetOccupiedStatus();
	bool CheckForCurrentTetromino();
	
	UMaterialInterface* CurrentMaterial();
	void SetMaterial(UMaterialInterface* newMaterial);	
};
