// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/Pawn.h"

#include "Cell.h"
#include "EndScreen.h"
#include "ScoreTest.h"
#include "StartMenu.h"
#include "Tetrominos.h"
#include "Timer.h"
#include "Grid.generated.h"

UCLASS()
class TETRISPROJECT_API AGrid : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* borderMaterial;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* colourMaterials[5];
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> playerOneScoreWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> playerTwoScoreWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> mainMenuWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> timerWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> endScreenWidget;	
	
private:
	//Functions and variables required to set up and start the game
	static const int height = 24;
	static const int width = 23;
	ACell* playerOneGrid[width][height];
	ACell* playerTwoGrid[width][height];
	void CreateGrid(ACell* (*grid)[width][height], FVector offset, ACell* (*waitGrids)[4][8][8],
	                ACell* (*holdGrid)[8][8]);
	void SetUpPlayerOne();
	void SetUpPlayerTwo();
	void SetUpGrids(ATetrominos* * currentTetromino, ACell* (*grid)[width][height], ACell* (*startPos)[4][4],
	                ACell* (*waitGrids)[4][8][8], ACell* (*waitPositions)[4][4][4], ATetrominos* (*nextTetrominos)[4],
	                ACell* (*holdGrid)[8][8], ACell* (*holdPosition)[4][4], ATetrominos* *holdTetromino);
	UFUNCTION()
	void SetUpGame();
	ACell* playerOneStartPos[4][4];
	ACell* playerTwoStartPos[4][4];
	bool CheckStartPositionEmpty(ATetrominos* * currentTetromino, ACell* (*tetrominoStartPos)[4][4]);

	//Current Tetromino variables
	ATetrominos* playerOneCurrentTetromino;
	ATetrominos* playerTwoCurrentTetromino;
	ACell* playerOneTempSample[4][4];
	ACell* playerTwoTempSample[4][4];
	int GetXIndex(ACell* sampleCell, ACell* (*grid)[width][height]);
	int GetYIndex(ACell* sampleCell, ACell* (*grid)[width][height]);

	//Player movements
	void PlayerOneMoveLeft();
	void PlayerOneMoveRight();
	void PlayerOneMoveDown();
	void PlayerTwoMoveLeft();
	void PlayerTwoMoveRight();
	void PlayerTwoMoveDown();
	void PlayerOneMoveToHold();
	void PlayerTwoMoveToHold();
	void MoveLeft(ATetrominos* * currentTetromino, ACell* (*tempSample)[4][4], ACell* (*grid)[width][height],
	              bool* isPlaying);
	void MoveRight(ATetrominos* * currentTetromino, ACell* (*tempSample)[4][4], ACell* (*grid)[width][height],
	               bool* isPlaying);
	void MoveDown(ATetrominos* * currentTetromino, ACell* (*tempSample)[4][4], ACell* (*grid)[width][height],
	              ACell* (*startPos)[4][4], bool* isPlaying, ATetrominos* (*nextTetrominos)[4],
	              ACell* (*waitPositions)[4][4][4], float* waitTime, float* elapseTime, int64* score, UScoreTest* *scoreText);

	//Swapping current tetromio to hold and swapping hold tetromio to currently playing
	void MoveToHold(ATetrominos* * currentTetromino, ATetrominos* * holdTetromino, ACell* (*holdPosition)[4][4],
	                ACell* (*startPos)[4][4], bool* isPlaying, ATetrominos* (*nextTetrominos)[4],
	                ACell* (*waitPositions)[4][4][4]);

	//Rotations
	void PlayerOneRotateShapeClockwise();
	void PlayerOneRotateShapeAntiClockwise();
	void PlayerTwoRotateShapeClockwise();
	void PlayerTwoRotateShapeAntiClockwise();
	void RotateShapeClockwise(ATetrominos* * currentTetromino, bool* isPlaying);
	void RotateShapeAntiClockwise(ATetrominos* * currentTetromino, bool* isPlaying);

	//These are ran when a tetromino can no longer move down anymore
	void CheckForLine(ATetrominos* * currentTetromino, ACell* (*grid)[width][height], ACell* (*tetrominoStartPos)[4][4],
	                  ATetrominos* (*nextTetrominos)[4], ACell* (*waitPositions)[4][4][4], bool* isPlaying,
	                  float* waitTime, float* elapseTime, int64* score, UScoreTest* *scoreText);
	void RemoveLine(int row, ATetrominos* * currentTetromino, ACell* (*grid)[width][height],
	                ACell* (*tetrominoStartPos)[4][4], ATetrominos* (*nextTetrominos)[4],
	                ACell* (*waitPositions)[4][4][4], bool* isPlaying, float* waitTime, float* elapseTime, int64* score, UScoreTest* *scoreText);

	//Game loop variables
	bool playerOneIsPlaying;
	float playerOneWaitTime;
	float playerOneElapseTime;
	bool playerTwoIsPlaying;
	float playerTwoWaitTime;
	float playerTwoElapseTime;

	//Tetromino Queue values
	ACell* playerOneWaitGrids[4][8][8];
	ACell* playerOnewaitPositions[4][4][4];
	ATetrominos* playerOnenextTetrominos[4];
	ACell* playerTwoWaitGrids[4][8][8];
	ACell* playerTwowaitPositions[4][4][4];
	ATetrominos* playerTwonextTetrominos[4];

	//Hold values
	ACell* playerOneHoldGrid[8][8];
	ACell* playerOneHoldPosition[4][4];
	ATetrominos* playerOneHoldTetromino;
	ACell* playerTwoHoldGrid[8][8];
	ACell* playerTwoHoldPosition[4][4];
	ATetrominos* playerTwoHoldTetromino;

	//Score variables
	int64 playerOneScore;
	int64 playerTwoScore;
	UScoreTest* playerOneScoreText;
	UScoreTest* playerTwoScoreText;

	//Start Menu Varabiles
	UStartMenu* startMenu;

	//Timer variables and functions
	int32 countdownTime = 300; //5min in seconds
	FTimerHandle countdownTimer;
	UTimer* timerDisplay;
	void UpdateTimer();

	//End Screen Variables
	UEndScreen* endScreen;
	UFUNCTION()
	void RestartGame();
};
