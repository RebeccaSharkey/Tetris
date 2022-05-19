// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"
#include <string>

// Sets default values
ACell::ACell()
{
	isOccupied = false;
	isCurrentTetromino = false;
	
	cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	cube->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeVisualAsset.Succeeded())
	{
		cube->SetStaticMesh(CubeVisualAsset.Object);
		cube->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		cube->SetWorldScale3D(FVector(0.05f));		
		auto material = cube->GetMaterial(0);
		dynamicMaterial = UMaterialInstanceDynamic::Create(material, NULL);
		cube->SetMaterial(0, dynamicMaterial);		
	}
}


void ACell::SetOccupiedStatus(bool newState, bool currentTetromino, UMaterialInterface* newMaterial)
{	
	isOccupied = newState;
	isCurrentTetromino = currentTetromino;
	if(newMaterial != NULL )
	{		
		currentMaterial = newMaterial;
		SetMaterial(newMaterial);
	}
	SetMesh();
}

bool ACell::GetOccupiedStatus()
{
	return isOccupied;
}

bool ACell::CheckForCurrentTetromino()
{
	return isCurrentTetromino;
}

UMaterialInterface* ACell::CurrentMaterial()
{
	return currentMaterial;
}

void ACell::SetMesh()
{
	if(isOccupied)
	{
		cube->SetVisibility(true);
	}
	else
	{
		cube->SetVisibility(false);
	}
}

void ACell::SetMaterial(UMaterialInterface* newMaterial)
{
	dynamicMaterial = UMaterialInstanceDynamic::Create(newMaterial, NULL);
	cube->SetMaterial(0, dynamicMaterial); 
}

