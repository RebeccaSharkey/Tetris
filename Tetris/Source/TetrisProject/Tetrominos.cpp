// Fill out your copyright notice in the Description page of Project Settings.


#include "Tetrominos.h"
#include <algorithm>
#include <memory>

// Sets default values
ATetrominos::ATetrominos()
{ 	

}

// Called when the game starts or when spawned
void ATetrominos::BeginPlay()
{
	Super::BeginPlay();
	
	isCurrentTetromino = false;
	
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			shape[i][j] = 0;
		}
	}
	
	SetShape();
}

void ATetrominos::SetUpTetromino(ACell* (*gridPostions)[4][4], bool state, UMaterialInterface* current)
{
	isCurrentTetromino = state;
	currentMaterial = current;
	SetPostion(gridPostions);
}

void ATetrominos::SetPostion(ACell* (*gridPostions)[4][4])
{
	
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{			
			if(shape[i][j] == 1)
			{
				(*gridPostions)[i][j]->SetOccupiedStatus(true, isCurrentTetromino, currentMaterial);
			}
			else
			{
				if(isCurrentTetromino)
				{					
					if(!(*gridPostions)[i][j]->GetOccupiedStatus())
					{
						(*gridPostions)[i][j]->SetOccupiedStatus(false, isCurrentTetromino, NULL);
					}
				}
			}
		}
	}
		
	//memcpy (&currentPostion, &gridPostions, sizeof gridPostions);
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			currentPostion[i][j] = (*gridPostions)[i][j];
		}
	}
	
}

ACell* ATetrominos::GetPosition()
{	
	return reinterpret_cast<ACell*>(currentPostion);
}

void ATetrominos::DeleteOldPosition()
{	
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{			
			if(shape[i][j] == 1)
			{
				currentPostion[i][j]->SetOccupiedStatus(false, false, NULL);
			}
			else
			{
				if(!currentPostion[i][j]->GetOccupiedStatus())
				{
					currentPostion[i][j]->SetOccupiedStatus(false, false, NULL);					
				}
			}
		}
	}
}

int ATetrominos::CheckForShape(int x, int y)
{
	return shape[x][y];
}

void ATetrominos::RotateShape(int direction)
{	
	switch (direction)
	{
	//Clockwise Rotation
		case 1:
			for(int i = 0; i < 4; i++)
			{
				for(int j = 0; j < 4; j++)
				{
					tempShape[i][j] = shape[i][j];
				}
			}
		
			for (int i = 0; i < 2; i++)
			{
				for (int j = i; j < 4 - i - 1; j++)
				{
					int temp = tempShape[i][j];
					tempShape[i][j] = tempShape[3 - j][i];
					tempShape[3- j][i] = tempShape[3 - i][3 - j];
					tempShape[3 - i][3 - j] = tempShape[j][3 - i];
					tempShape[j][3 - i] = temp;
				}
			}
			
		break;

		//Counter Clockwise Rotation
		case  2:
			for(int i = 0; i < 4; i++)
			{
				for(int j = 0; j < 4; j++)
				{
					tempShape[i][j] = shape[i][j];
				}
			}
		
			for (int i = 0; i < 2; i++)
			{
				for (int j = i; j < 4 - i - 1; j++)
				{
					int temp = tempShape[i][j];
					tempShape[i][j] = tempShape[j][3 - i];
					tempShape[j][3 - i] = tempShape[3 - i][3 - j];
					tempShape[3 - i][3 - j]= tempShape[3 - j][i];
					tempShape[3 - j][i] = temp;
				}
			}
		break;
	}
}

void ATetrominos::SetRotation()
{	
	DeleteOldPosition();
	memcpy(shape, tempShape, sizeof(shape));
	SetPostion(&currentPostion);
}

void ATetrominos::RemoveCurrentTetrominoMarker()
{
	isCurrentTetromino = false;
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{			
			if(shape[i][j] == 1)
			{
				currentPostion[i][j]->SetOccupiedStatus(true, isCurrentTetromino, currentMaterial);
			}
			else
			{
				if(!currentPostion[i][j]->GetOccupiedStatus())
				{
					currentPostion[i][j]->SetOccupiedStatus(false, isCurrentTetromino, NULL);	
				}
			}
		}
	}
}

UMaterialInterface* ATetrominos::GetCurrentMaterial()
{
	return currentMaterial;
}


void ATetrominos::SetShape()
{
	int randomShape = FMath::RandRange( 0, 6);
	switch (randomShape)
	{
	// L-Shape
	case 0:
		shape[0][3] = 0; shape[1][3] = 0; shape[2][3] = 0; shape[3][3] = 0; 
		shape[0][2] = 0; shape[1][2] = 1; shape[2][2] = 0; shape[3][2] = 0; 
		shape[0][1] = 0; shape[1][1] = 1; shape[2][1] = 0; shape[3][1] = 0; 
		shape[0][0] = 0; shape[1][0] = 1; shape[2][0] = 1; shape[3][0] = 0; 
		break;
	//J-Shape
	case 1:
		shape[0][3] = 0; shape[1][3] = 0; shape[2][3] = 0; shape[3][3] = 0; 
		shape[0][2] = 0; shape[1][2] = 0; shape[2][2] = 1; shape[3][2] = 0; 
		shape[0][1] = 0; shape[1][1] = 0; shape[2][1] = 1; shape[3][1] = 0; 
		shape[0][0] = 0; shape[1][0] = 1; shape[2][0] = 1; shape[3][0] = 0; 
		break;
	//O-Shape
	case 2:
		shape[0][3] = 0; shape[1][3] = 0; shape[2][3] = 0; shape[3][3] = 0; 
		shape[0][2] = 0; shape[1][2] = 1; shape[2][2] = 1; shape[3][2] = 0; 
		shape[0][1] = 0; shape[1][1] = 1; shape[2][1] = 1; shape[3][1] = 0; 
		shape[0][0] = 0; shape[1][0] = 0; shape[2][0] = 0; shape[3][0] = 0; 		
		break;
	//I-Shape
	case 3:
		shape[0][3] = 0; shape[1][3] = 0; shape[2][3] = 1; shape[3][3] = 0; 
		shape[0][2] = 0; shape[1][2] = 0; shape[2][2] = 1; shape[3][2] = 0; 
		shape[0][1] = 0; shape[1][1] = 0; shape[2][1] = 1; shape[3][1] = 0; 
		shape[0][0] = 0; shape[1][0] = 0; shape[2][0] = 1; shape[3][0] = 0;
		break;
	//T-Shape
	case 4:
		shape[0][3] = 0; shape[1][3] = 0; shape[2][3] = 0; shape[3][3] = 0; 
		shape[0][2] = 0; shape[1][2] = 0; shape[2][2] = 0; shape[3][2] = 0; 
		shape[0][1] = 1; shape[1][1] = 1; shape[2][1] = 1; shape[3][1] = 0; 
		shape[0][0] = 0; shape[1][0] = 1; shape[2][0] = 0; shape[3][0] = 0;
		break;
	//S-Shape
	case 5:
		shape[0][3] = 0; shape[1][3] = 0; shape[2][3] = 0; shape[3][3] = 0; 
		shape[0][2] = 0; shape[1][2] = 0; shape[2][2] = 0; shape[3][2] = 0; 
		shape[0][1] = 0; shape[1][1] = 1; shape[2][1] = 1; shape[3][1] = 0; 
		shape[0][0] = 1; shape[1][0] = 1; shape[2][0] = 0; shape[3][0] = 0;
		break;
	//Z-Shape
	case 6:
		shape[0][3] = 0; shape[1][3] = 0; shape[2][3] = 0; shape[3][3] = 0; 
		shape[0][2] = 0; shape[1][2] = 0; shape[2][2] = 0; shape[3][2] = 0; 
		shape[0][1] = 1; shape[1][1] = 1; shape[2][1] = 0; shape[3][1] = 0; 
		shape[0][0] = 0; shape[1][0] = 1; shape[2][0] = 1; shape[3][0] = 0;
		break;
	//Test Shape
	case 7:
		shape[0][3] = 1; shape[1][3] = 1; shape[2][3] = 1; shape[3][3] = 1; 
		shape[0][2] = 1; shape[1][2] = 1; shape[2][2] = 1; shape[3][2] = 1; 
		shape[0][1] = 1; shape[1][1] = 1; shape[2][1] = 1; shape[3][1] = 1; 
		shape[0][0] = 1; shape[1][0] = 1; shape[2][0] = 1; shape[3][0] = 1;
		break;
	}	

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			tempShape[i][j] = shape[i][j];
		}
	}
	
}

