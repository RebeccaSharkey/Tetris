// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include <algorithm>
#include <string>

#include "Components/Button.h"
#include "Components/TextBlock.h"
//#include "EnvironmentQuery/EnvQueryTypes.h"

// Sets default values
AGrid::AGrid()
{
	//Adapted from Ferneyhough, (2022)	
	PrimaryActorTick.bCanEverTick = true;

	//Takes input from player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	//Creates Camera
	UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurCamera->SetupAttachment(RootComponent);
	OurCamera->SetRelativeLocation(FVector(-150.f, 0.f, 65.f));
	OurCamera->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	//End of adapted code

	playerOneIsPlaying = false;
	playerTwoIsPlaying = false;
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();

	this->SetActorLocation(FVector(0.f, 0.f, 0.f));

	CreateGrid(&playerOneGrid, FVector(0, -75, 0), &playerOneWaitGrids, &playerOneHoldGrid);
	CreateGrid(&playerTwoGrid, FVector(0, 75, 0), &playerTwoWaitGrids, &playerTwoHoldGrid);

	SetUpGrids(&playerOneCurrentTetromino, &playerOneGrid, &playerOneStartPos, &playerOneWaitGrids,
			   &playerOnewaitPositions, &playerOnenextTetrominos, &playerOneHoldGrid, &playerOneHoldPosition,
			   &playerOneHoldTetromino);

	SetUpGrids(&playerTwoCurrentTetromino, &playerTwoGrid, &playerTwoStartPos, &playerTwoWaitGrids,
			   &playerTwowaitPositions, &playerTwonextTetrominos, &playerTwoHoldGrid, &playerTwoHoldPosition,
			   &playerTwoHoldTetromino);
	
	//SetUpGame();
	if(startMenu == nullptr)
	{
		startMenu = Cast<UStartMenu>(CreateWidget(GetWorld(), mainMenuWidget));		
	}
	if (startMenu != nullptr)
	{
		startMenu->AddToViewport();
		startMenu->playButton->OnClicked.AddUniqueDynamic(this, &AGrid::SetUpGame);
	}
}

void AGrid::CreateGrid(ACell* (*grid)[width][height], FVector offset, ACell* (*waitGrids)[4][8][8],
                       ACell* (*holdGrid)[8][8])
{
	//Creates an array of cells for the game
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			(*grid)[i][j] = (ACell*)GWorld->SpawnActor(ACell::StaticClass());
			(*grid)[i][j]->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Cell"));
			(*grid)[i][j]->SetActorLocation(FVector(offset.X, offset.Y + (5 * i) - ((width * 5) / 2),
			                                        offset.Z + (5 * j) - 15));
		}
	}

	//Creates an Array of cells for the queue
	for (int k = 0; k < 4; k++)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				(*waitGrids)[k][i][j] = (ACell*)GWorld->SpawnActor(ACell::StaticClass());
				(*waitGrids)[k][i][j]->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform,
				                                     TEXT("Wait"));
				(*waitGrids)[k][i][j]->SetActorLocation(FVector(offset.X, offset.Y + 40 + (2.5f * i),
				                                                offset.Z + 22.5f + (k * 17.5f) + (2.5f * j) - 15));
				(*waitGrids)[k][i][j]->SetActorScale3D(FVector(0.025f));
			}
		}
	}

	//Creates an array of cells for the hold position
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			(*holdGrid)[i][j] = (ACell*)GWorld->SpawnActor(ACell::StaticClass());
			(*holdGrid)[i][j]->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform,
			                                 TEXT("Hold"));
			(*holdGrid)[i][j]->SetActorLocation(FVector(offset.X, offset.Y - 60 + (2.5F * i),
			                                            offset.Z + 99 + (2.5F * j) - 15));
			(*holdGrid)[i][j]->SetActorScale3D(FVector(0.025f));
		}
	}
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (playerOneIsPlaying)
	{
		playerOneElapseTime += DeltaTime;
		if (playerOneElapseTime >= playerOneWaitTime)
		{
			playerOneElapseTime = 0.f;
			MoveDown(&playerOneCurrentTetromino, &playerOneTempSample, &playerOneGrid, &playerOneStartPos,
			         &playerOneIsPlaying,
			         &playerOnenextTetrominos, &playerOnewaitPositions, &playerOneWaitTime, &playerOneElapseTime, &playerOneScore, &playerOneScoreText);
		}
	}

	if (playerTwoIsPlaying)
	{
		playerTwoElapseTime += DeltaTime;
		if (playerTwoElapseTime >= playerTwoWaitTime)
		{
			playerTwoElapseTime = 0.f;
			MoveDown(&playerTwoCurrentTetromino, &playerTwoTempSample, &playerTwoGrid, &playerTwoStartPos,
			         &playerTwoIsPlaying,
			         &playerTwonextTetrominos, &playerTwowaitPositions, &playerTwoWaitTime, &playerTwoElapseTime, &playerTwoScore, &playerTwoScoreText);
		}
	}
}

// Called to bind functionality to input
void AGrid::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Player One
	InputComponent->BindAction("MoveDown", IE_Pressed, this, &AGrid::PlayerOneMoveDown);
	InputComponent->BindAction("MoveLeft", IE_Pressed, this, &AGrid::PlayerOneMoveLeft);
	InputComponent->BindAction("MoveRight", IE_Pressed, this, &AGrid::PlayerOneMoveRight);
	InputComponent->BindAction("ClockwiseRotate", IE_Pressed, this, &AGrid::PlayerOneRotateShapeClockwise);
	InputComponent->BindAction("AntiClockwiseRotate", IE_Pressed, this, &AGrid::PlayerOneRotateShapeAntiClockwise);
	InputComponent->BindAction("Hold", IE_Pressed, this, &AGrid::PlayerOneMoveToHold);

	//Player Two
	InputComponent->BindAction("MoveDown2", IE_Pressed, this, &AGrid::PlayerTwoMoveDown);
	InputComponent->BindAction("MoveLeft2", IE_Pressed, this, &AGrid::PlayerTwoMoveLeft);
	InputComponent->BindAction("MoveRight2", IE_Pressed, this, &AGrid::PlayerTwoMoveRight);
	InputComponent->BindAction("ClockwiseRotate2", IE_Pressed, this, &AGrid::PlayerTwoRotateShapeClockwise);
	InputComponent->BindAction("AntiClockwiseRotate2", IE_Pressed, this, &AGrid::PlayerTwoRotateShapeAntiClockwise);
	InputComponent->BindAction("Hold2", IE_Pressed, this, &AGrid::PlayerTwoMoveToHold);
}

void AGrid::SetUpGame()
{
	if (startMenu != nullptr)
	{
		startMenu->RemoveFromViewport();
	}
	
	SetUpPlayerOne();
	SetUpPlayerTwo();

	playerOneScore = 0;
	playerTwoScore = 0;

	if(playerOneScoreText == nullptr)
	{
		playerOneScoreText = Cast<UScoreTest>(CreateWidget(GetWorld(), playerOneScoreWidget));		
	}
	if (playerOneScoreText != nullptr)
	{
		playerOneScoreText->AddToViewport();
		playerOneScoreText->SetScore(playerOneScore);
	}

	if (playerTwoScoreText == nullptr)
	{		
		playerTwoScoreText = Cast<UScoreTest>(CreateWidget(GetWorld(), playerTwoScoreWidget));		
	}
	if (playerTwoScoreText != nullptr)
	{
		playerTwoScoreText->AddToViewport();
		playerTwoScoreText->SetScore(playerOneScore);
	}

	if(timerDisplay == nullptr)
	{
		timerDisplay = Cast<UTimer>(CreateWidget(GetWorld(), timerWidget));
	}
	if (timerDisplay != nullptr)
	{
		timerDisplay->AddToViewport();
		timerDisplay->SetTimer(countdownTime);
		GetWorldTimerManager().SetTimer(countdownTimer, this, &AGrid::UpdateTimer, 1.0f, true);
	}
}

void AGrid::SetUpPlayerOne()
{
	SetUpGrids(&playerOneCurrentTetromino, &playerOneGrid, &playerOneStartPos, &playerOneWaitGrids,
	           &playerOnewaitPositions, &playerOnenextTetrominos, &playerOneHoldGrid, &playerOneHoldPosition,
	           &playerOneHoldTetromino);

	playerOneWaitTime = 1.f;
	playerOneElapseTime = 0.f;
	playerOneIsPlaying = true;
}

void AGrid::SetUpPlayerTwo()
{
	SetUpGrids(&playerTwoCurrentTetromino, &playerTwoGrid, &playerTwoStartPos, &playerTwoWaitGrids,
	           &playerTwowaitPositions, &playerTwonextTetrominos, &playerTwoHoldGrid, &playerTwoHoldPosition,
	           &playerTwoHoldTetromino);

	playerTwoWaitTime = 1.f;
	playerTwoElapseTime = 0.f;
	playerTwoIsPlaying = true;
}


void AGrid::SetUpGrids(ATetrominos* * currentTetromino, ACell* (*grid)[width][height], ACell* (*startPos)[4][4],
                       ACell* (*waitGrids)[4][8][8], ACell* (*waitPositions)[4][4][4],
                       ATetrominos* (*nextTetrominos)[4], ACell* (*holdGrid)[8][8], ACell* (*holdPosition)[4][4],
                       ATetrominos* * holdTetromino)
{
	//Sets up the cells for each postion on the grid
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			//Creates borders
			if (j == 4 && (i >= 4 && i <= width - 5))
			{
				(*grid)[i][j]->SetOccupiedStatus(true, false, borderMaterial);
			}
			//Creates Play Area
			else
			{
				if ((i == 4 || i == width - 5) && j > 4)
				{
					(*grid)[i][j]->SetOccupiedStatus(true, false, borderMaterial);
				}
				else
				{
					(*grid)[i][j]->SetOccupiedStatus(false, false, NULL);
				}
			}
		}
	}

	//Sets up start positions
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			(*startPos)[i][j] = (*grid)[i + ((width / 2) - 1)][j + (height - 4)];
		}
	}

	//Sets up the tetromino queue grids to the right of the play grid
	for (int k = 0; k < 4; k++)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				//Creates borders
				if (j == 0 || j == 7)
				{
					(*waitGrids)[k][i][j]->SetOccupiedStatus(true, false, borderMaterial);
				}
				else
				{
					if (i == 0 || i == 7)
					{
						(*waitGrids)[k][i][j]->SetOccupiedStatus(true, false, borderMaterial);
					}
					else
					{
						(*waitGrids)[k][i][j]->SetOccupiedStatus(false, false, NULL);

						if ((i >= 2 && i <= 5) && (j >= 2 && j <= 5))
						{
							(*waitPositions)[k][i - 2][j - 2] = (*waitGrids)[k][i][j];
						}
					}
				}
			}
		}
	}

	(*currentTetromino) = (ATetrominos*)GWorld->SpawnActor(ATetrominos::StaticClass());
	(*currentTetromino)->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Tettromino"));
	(*currentTetromino)->SetActorLocation(FVector(0, 0, 0));

	int randomNumber = FMath::RandRange(0, 4);
	(*currentTetromino)->SetUpTetromino(startPos, true, colourMaterials[randomNumber]);

	for (int k = 0; k < 4; k++)
	{
		(*nextTetrominos)[k] = (ATetrominos*)GWorld->SpawnActor(ATetrominos::StaticClass());
		(*nextTetrominos)[k]->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Tettromino"));
		(*nextTetrominos)[k]->SetActorLocation(FVector(0, 0, 0));
		(*nextTetrominos)[k]->SetUpTetromino(&(*waitPositions)[3 - k], false, colourMaterials[FMath::RandRange(0, 4)]);
	}

	holdTetromino = nullptr;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			//Creates borders
			if (j == 0 || j == 7)
			{
				(*holdGrid)[i][j]->SetOccupiedStatus(true, false, borderMaterial);
			}
			else
			{
				if (i == 0 || i == 7)
				{
					(*holdGrid)[i][j]->SetOccupiedStatus(true, false, borderMaterial);
				}
				else
				{
					(*holdGrid)[i][j]->SetOccupiedStatus(false, false, NULL);

					if ((i >= 2 && i <= 5) && (j >= 2 && j <= 5))
					{
						(*holdPosition)[i - 2][j - 2] = (*holdGrid)[i][j];
					}
				}
			}
		}
	}
}


void AGrid::PlayerOneMoveDown()
{
	MoveDown(&playerOneCurrentTetromino, &playerOneTempSample, &playerOneGrid, &playerOneStartPos, &playerOneIsPlaying,
	         &playerOnenextTetrominos, &playerOnewaitPositions, &playerOneWaitTime, &playerOneElapseTime, &playerOneScore, &playerOneScoreText);
}

void AGrid::PlayerOneMoveLeft()
{
	MoveLeft(&playerOneCurrentTetromino, &playerOneTempSample, &playerOneGrid, &playerOneIsPlaying);
}

void AGrid::PlayerOneMoveRight()
{
	MoveRight(&playerOneCurrentTetromino, &playerOneTempSample, &playerOneGrid, &playerOneIsPlaying);
}

void AGrid::PlayerOneRotateShapeClockwise()
{
	RotateShapeClockwise(&playerOneCurrentTetromino, &playerOneIsPlaying);
}

void AGrid::PlayerOneRotateShapeAntiClockwise()
{
	RotateShapeAntiClockwise(&playerOneCurrentTetromino, &playerOneIsPlaying);
}

void AGrid::PlayerOneMoveToHold()
{
	MoveToHold(&playerOneCurrentTetromino, &playerOneHoldTetromino, &playerOneHoldPosition, &playerOneStartPos,
	           &playerOneIsPlaying, &playerOnenextTetrominos, &playerOnewaitPositions);
}


void AGrid::PlayerTwoMoveDown()
{
	MoveDown(&playerTwoCurrentTetromino, &playerTwoTempSample, &playerTwoGrid, &playerTwoStartPos, &playerTwoIsPlaying,
	         &playerTwonextTetrominos, &playerTwowaitPositions, &playerTwoWaitTime, &playerTwoElapseTime, &playerTwoScore, &playerTwoScoreText);
}

void AGrid::PlayerTwoMoveLeft()
{
	MoveLeft(&playerTwoCurrentTetromino, &playerTwoTempSample, &playerTwoGrid, &playerTwoIsPlaying);
}

void AGrid::PlayerTwoMoveRight()
{
	MoveRight(&playerTwoCurrentTetromino, &playerTwoTempSample, &playerTwoGrid, &playerTwoIsPlaying);
}

void AGrid::PlayerTwoRotateShapeClockwise()
{
	RotateShapeClockwise(&playerTwoCurrentTetromino, &playerTwoIsPlaying);
}

void AGrid::PlayerTwoRotateShapeAntiClockwise()
{
	RotateShapeAntiClockwise(&playerTwoCurrentTetromino, &playerTwoIsPlaying);
}

void AGrid::PlayerTwoMoveToHold()
{
	MoveToHold(&playerTwoCurrentTetromino, &playerTwoHoldTetromino, &playerTwoHoldPosition, &playerTwoStartPos,
	           &playerTwoIsPlaying, &playerTwonextTetrominos, &playerTwowaitPositions);
}

void AGrid::MoveDown(ATetrominos* * currentTetromino, ACell* (*tempSample)[4][4], ACell* (*grid)[width][height],
                     ACell* (*startPos)[4][4], bool* isPlaying, ATetrominos* (*nextTetrominos)[4],
                     ACell* (*waitPositions)[4][4][4], float* waitTime, float* elapseTime, int64* score, UScoreTest* *scoreText)
{
	if (*isPlaying)
	{
		memcpy(tempSample, (*currentTetromino)->GetPosition(), sizeof(tempSample));

		int xOffset = GetXIndex((*tempSample)[0][0], &(*grid));
		int yOffset = GetYIndex((*tempSample)[0][0], &(*grid));

		if (yOffset != 0 || xOffset != NULL)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					//Checks if there is a shape in this part of the grid
					if ((*currentTetromino)->CheckForShape(i, j) == 1)
					{
						//If the grid is occupied at slot below of the tetromino then...
						if ((*grid)[(xOffset + i)][(yOffset + j) - 1]->GetOccupiedStatus())
						{
							//If the next occupied spot is occupied by the current tetromino then ignore and move...
							if ((*grid)[(xOffset + i)][(yOffset + j) - 1]->CheckForCurrentTetromino())
							{
								(*tempSample)[i][j] = (*grid)[(xOffset + i)][(yOffset + j) - 1];
							}
							//If the next slot is occupied with something other than current shape then player can't move here as there is something blocking the path
							else
							{
								*isPlaying = false;
								(*currentTetromino)->RemoveCurrentTetrominoMarker();
								CheckForLine(currentTetromino, grid, startPos, nextTetrominos, waitPositions,
								             isPlaying, waitTime, elapseTime, score, scoreText);
								return;
							}
						}

						//If the next slot is not occupied then shape can move here
						else
						{
							(*tempSample)[i][j] = (*grid)[(xOffset + i)][(yOffset + j) - 1];
						}
					}

					//If the shape isn't in this part of the grid then move down
					else
					{
						(*tempSample)[i][j] = (*grid)[(xOffset + i)][(yOffset + j) - 1];
					}
				}
			}

			(*currentTetromino)->DeleteOldPosition();
			(*currentTetromino)->SetPostion(tempSample);
		}
	}
}

void AGrid::MoveLeft(ATetrominos* * currentTetromino, ACell* (*tempSample)[4][4], ACell* (*grid)[width][height],
                     bool* isPlaying)
{
	if (isPlaying)
	{
		*isPlaying = true;
		memcpy(tempSample, (*currentTetromino)->GetPosition(), sizeof(tempSample));

		int xOffset = GetXIndex((*tempSample)[0][0], &(*grid));
		int yOffset = GetYIndex((*tempSample)[0][0], &(*grid));

		if (xOffset != 0)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 3; j >= 0; j--)
				{
					//Checks if there is a shape in this part of the grid
					if ((*currentTetromino)->CheckForShape(i, j) == 1)
					{
						//If the grid is occupied at slot right of the tetromino then...
						if ((*grid)[(xOffset + i) - 1][(yOffset + j)]->GetOccupiedStatus())
						{
							//If the next occupied spot is occupied by the current tetromino then ignore and move...
							if ((*grid)[(xOffset + i) - 1][(yOffset + j)]->CheckForCurrentTetromino())
							{
								(*tempSample)[i][j] = (*grid)[(xOffset + i) - 1][(yOffset + j)];
							}
							else
							{
								return;
							}
						}

						//If the next slot is not occupied then shape can move here
						else
						{
							(*tempSample)[i][j] = (*grid)[(xOffset + i) - 1][(yOffset + j)];
						}
					}

					//If the shape isn't in this part of the grid then move down
					else
					{
						(*tempSample)[i][j] = (*grid)[(xOffset + i) - 1][(yOffset + j)];
					}
				}
			}
			(*currentTetromino)->DeleteOldPosition();
			(*currentTetromino)->SetPostion(tempSample);
		}
		*isPlaying = true;
	}
}

void AGrid::MoveRight(ATetrominos* * currentTetromino, ACell* (*tempSample)[4][4], ACell* (*grid)[width][height],
                      bool* isPlaying)
{
	if (*isPlaying)
	{
		*isPlaying = false;
		memcpy(tempSample, (*currentTetromino)->GetPosition(), sizeof(tempSample));

		int xOffset = GetXIndex((*tempSample)[0][0], &(*grid));
		int yOffset = GetYIndex((*tempSample)[0][0], &(*grid));

		if (xOffset != width - 1)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 3; j >= 0; j--)
				{
					//Checks if there is a shape in this part of the grid
					if ((*currentTetromino)->CheckForShape(i, j) == 1)
					{
						//If the grid is occupied at slot right of the tetromino then...
						if ((*grid)[(xOffset + i) + 1][(yOffset + j)]->GetOccupiedStatus())
						{
							//If the next occupied spot is occupied by the current tetromino then ignore and move...
							if ((*grid)[(xOffset + i) + 1][(yOffset + j)]->CheckForCurrentTetromino())
							{
								(*tempSample)[i][j] = (*grid)[(xOffset + i) + 1][(yOffset + j)];
							}
							else
							{
								*isPlaying = true;
								return;
							}
						}

						//If the next slot is not occupied then shape can move here
						else
						{
							(*tempSample)[i][j] = (*grid)[(xOffset + i) + 1][(yOffset + j)];
						}
					}

					//If the shape isn't in this part of the grid then move down
					else
					{
						(*tempSample)[i][j] = (*grid)[(xOffset + i) + 1][(yOffset + j)];
					}
				}
			}
			(*currentTetromino)->DeleteOldPosition();
			(*currentTetromino)->SetPostion(tempSample);
		}
		*isPlaying = true;
	}
}

void AGrid::MoveToHold(ATetrominos* * currentTetromino, ATetrominos* * holdTetromino, ACell* (*holdPosition)[4][4],
                       ACell* (*tetrominoStartPos)[4][4], bool* isPlaying, ATetrominos* (*nextTetrominos)[4],
                       ACell* (*waitPositions)[4][4][4])
{
	if(*isPlaying)
	{
		*isPlaying = false;

		//If the player isn't currently holding anything
		if (*holdTetromino == nullptr)
		{
			*holdTetromino = *currentTetromino;
			(*currentTetromino)->DeleteOldPosition();
			UMaterialInterface* tempMat = (*currentTetromino)->GetCurrentMaterial();
			(*holdTetromino)->SetUpTetromino(holdPosition, false, tempMat);

			(*currentTetromino) = (*nextTetrominos)[0];
			UMaterialInterface* temp = (*nextTetrominos)[0]->GetCurrentMaterial();

			if (CheckStartPositionEmpty(currentTetromino, tetrominoStartPos))
			{
				for (int i = 0; i < 4; i++)
				{
					if (i != 3)
					{
						if (i == 0)
						{
							(*nextTetrominos)[i]->DeleteOldPosition();
						}
						(*nextTetrominos)[i] = (*nextTetrominos)[i + 1];
						(*nextTetrominos)[i]->DeleteOldPosition();
						(*nextTetrominos)[i]->SetPostion(&(*waitPositions)[3 - i]);
					}
					else
					{
						(*nextTetrominos)[i] = (ATetrominos*)GWorld->SpawnActor(ATetrominos::StaticClass());
						(*nextTetrominos)[i]->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform,
															TEXT("Tettromino"));
						(*nextTetrominos)[i]->SetActorLocation(FVector(0, 0, 0));
						int randomNumber = FMath::RandRange(0, 4);
						(*nextTetrominos)[i]->
							SetUpTetromino(&(*waitPositions)[3 - i], false, colourMaterials[randomNumber]);
					}
				}
				(*currentTetromino)->SetUpTetromino(tetrominoStartPos, true, temp);
				*isPlaying = true;
			}
		}
		else
		{
			ATetrominos* temp = (*currentTetromino);
			UMaterialInterface* tempMat2 = temp->GetCurrentMaterial();

			(*currentTetromino)->DeleteOldPosition();
			(*holdTetromino)->DeleteOldPosition();

			(*currentTetromino) = (*holdTetromino);
			UMaterialInterface* tempMat = (*holdTetromino)->GetCurrentMaterial();
			(*currentTetromino)->SetUpTetromino(tetrominoStartPos, true, tempMat);

			*holdTetromino = temp;
			(*holdTetromino)->SetUpTetromino(holdPosition, false, tempMat2);
			*isPlaying = true;
		}
	}	
}


void AGrid::RotateShapeClockwise(ATetrominos* * currentTetromino, bool* isPlaying)
{
	if(*isPlaying)
	{		
		*isPlaying = false;
		ACell* tempPostion[4][4];
		memcpy(tempPostion, (*currentTetromino)->GetPosition(), sizeof(tempPostion));

		(*currentTetromino)->RotateShape(1);

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if ((*currentTetromino)->tempShape[i][j] == 1)
				{
					//Checks to see if there is an occupied space at shapes new position if there is then...
					if (tempPostion[i][j]->GetOccupiedStatus())
					{
						//Checks to see if the occupied space is part of the current playable tetromino if not then...
						if ((*currentTetromino)->CheckForShape(i, j) == 0)
						{
							*isPlaying = true;
							return;
						}
					}
				}
			}
		}
		(*currentTetromino)->SetRotation();
		*isPlaying = true;
	}
}

void AGrid::RotateShapeAntiClockwise(ATetrominos* * currentTetromino, bool* isPlaying)
{
	if(*isPlaying)
	{
		*isPlaying = false;
		ACell* tempPostion[4][4];
		memcpy(tempPostion, (*currentTetromino)->GetPosition(), sizeof(tempPostion));

		(*currentTetromino)->RotateShape(2);

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if ((*currentTetromino)->tempShape[i][j] == 1)
				{
					//Checks to see if there is an occupied space at shapes new position if there is then...
					if (tempPostion[i][j]->GetOccupiedStatus())
					{
						//Checks to see if the occupied space is part of the current playable tetromino if not then...
						if ((*currentTetromino)->CheckForShape(i, j) == 0)
						{
							*isPlaying = true;
							return;
						}
					}
				}
			}
		}
		(*currentTetromino)->SetRotation();
		*isPlaying = true;
	}	
}

int AGrid::GetXIndex(ACell* sampleCell, ACell* (*grid)[width][height])
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if ((*grid)[i][j] == sampleCell)
			{
				return i;
			}
		}
	}
	return NULL;
}

int AGrid::GetYIndex(ACell* sampleCell, ACell* (*grid)[width][height])
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if ((*grid)[i][j] == sampleCell)
			{
				return j;
			}
		}
	}
	return NULL;
}

void AGrid::CheckForLine(ATetrominos* *currentTetromino, ACell* (*grid)[width][height],
                         ACell* (*tetrominoStartPos)[4][4], ATetrominos* (*nextTetrominos)[4],
                         ACell* (*waitPositions)[4][4][4], bool* isPlaying, float* waitTime, float* elapseTime,
                         int64* score, UScoreTest* *scoreText)
{
	bool isLine = false;

	for (int j = 5; j < height; j++)
	{
		for (int i = 5; i <= width - 6; i++)
		{
			if (!(*grid)[i][j]->GetOccupiedStatus())
			{
				break;
			}

			if (i == (width - 6))
			{
				isLine = true;
				RemoveLine(j, currentTetromino, grid, tetrominoStartPos, nextTetrominos, waitPositions, isPlaying,
				           waitTime, elapseTime, score, scoreText);

				//Increases score
				*score += 100;
				if(*scoreText != nullptr)
				{
					(*scoreText)->SetScore(*score);
				}

				//increases speed of tetrominos
				if((*score % 100) == 0)
				{
					*waitTime -= 0.1f;
					if(*waitTime <= 0)
					{
						*waitTime = 0.1f;
					}
				}
			}
		}
	}

	if (!isLine)
	{
		(*currentTetromino) = (*nextTetrominos)[0];
		UMaterialInterface* temp = (*nextTetrominos)[0]->GetCurrentMaterial();

		if (CheckStartPositionEmpty(currentTetromino, tetrominoStartPos))
		{
			for (int i = 0; i < 4; i++)
			{
				if (i != 3)
				{
					if (i == 0)
					{
						(*nextTetrominos)[i]->DeleteOldPosition();
					}
					(*nextTetrominos)[i] = (*nextTetrominos)[i + 1];
					(*nextTetrominos)[i]->DeleteOldPosition();
					(*nextTetrominos)[i]->SetPostion(&(*waitPositions)[3 - i]);
				}
				else
				{
					(*nextTetrominos)[i] = (ATetrominos*)GWorld->SpawnActor(ATetrominos::StaticClass());
					(*nextTetrominos)[i]->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform,
					                                    TEXT("Tettromino"));
					(*nextTetrominos)[i]->SetActorLocation(FVector(0, 0, 0));
					int randomNumber = FMath::RandRange(0, 4);
					(*nextTetrominos)[i]->
						SetUpTetromino(&(*waitPositions)[3 - i], false, colourMaterials[randomNumber]);
				}
			}
			(*currentTetromino)->SetUpTetromino(tetrominoStartPos, true, temp);
			
			*elapseTime = 0.f;
			*isPlaying = true;
		}
		else
		{			
			if (tetrominoStartPos == &playerOneStartPos)
			{
				SetUpPlayerOne();
				*score -= 300;
				if(*score < 0)
				{
					*score = 0;
				}
				if(*scoreText != nullptr)
				{
					(*scoreText)->SetScore(*score);
				}
			}
			else
			{
				SetUpPlayerTwo();
				*score -= 300;
				if(*score < 0)
				{
					*score = 0;
				}
				if(*scoreText != nullptr)
				{
					(*scoreText)->SetScore(*score);
				}
			}
		}
	}
}

void AGrid::RemoveLine(int row, ATetrominos* * currentTetromino, ACell* (*grid)[width][height],
                       ACell* (*tetrominoStartPos)[4][4], ATetrominos* (*nextTetrominos)[4],
                       ACell* (*waitPositions)[4][4][4], bool* isPlaying, float* waitTime, float* elapseTime, int64* score, UScoreTest* *scoreText)
{
	for (int i = 5; i <= width - 6; i++)
	{
		(*grid)[i][row]->SetOccupiedStatus(false, false, NULL);
	}

	for (int i = 5; i <= width - 6; i++)
	{
		for (int j = row; j < height - 1; j++)
		{
			if ((*grid)[i][j + 1]->GetOccupiedStatus())
			{
				(*grid)[i][j]->SetOccupiedStatus(true, false, (*grid)[i][j + 1]->CurrentMaterial());
				(*grid)[i][j + 1]->SetOccupiedStatus(false, false, NULL);
			}
		}
	}

	CheckForLine(currentTetromino, &(*grid), &(*tetrominoStartPos), &(*nextTetrominos), &(*waitPositions), isPlaying,
	             waitTime, elapseTime, score, scoreText);
}

bool AGrid::CheckStartPositionEmpty(ATetrominos* * currentTetromino, ACell* (*tetrominoStartPos)[4][4])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if ((*currentTetromino)->tempShape[i][j] == 1)
			{
				if ((*tetrominoStartPos)[i][j]->GetOccupiedStatus())
				{
					return false;
				}
			}
		}
	}
	return true;
}

// Code Adapted From Rombauts, 2015 - 2020
void AGrid::UpdateTimer()
{
	--countdownTime;
	if (timerDisplay != nullptr)
	{
		timerDisplay->SetTimer(countdownTime);
	}
	if(countdownTime < 1)
	{
		GetWorldTimerManager().ClearTimer(countdownTimer);
		playerOneIsPlaying = false;
		playerTwoIsPlaying = false;
		
		if(endScreen == nullptr)
		{
			endScreen = Cast<UEndScreen>(CreateWidget(GetWorld(), endScreenWidget));		
		}
		if (endScreen != nullptr)
		{
			endScreen->playerOnescoreLabel->SetText(FText::AsNumber(playerOneScore));
			endScreen->playerTwoscoreLabel->SetText(FText::AsNumber(playerTwoScore));			
			endScreen->AddToViewport();
			endScreen->restartButton->OnClicked.AddUniqueDynamic(this, &AGrid::RestartGame);

			timerDisplay->RemoveFromViewport();
			playerOneScoreText->RemoveFromViewport();
			playerTwoScoreText->RemoveFromViewport();
		}
	}
}
//end of adapted code

void AGrid::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}
