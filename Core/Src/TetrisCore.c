/*
 * TetrisCore.c
 *
 *  Created on: Jun 27, 2022
 *      Author: James.Marshall
 */
#include <string.h>

#include <TetrisCore.h>
#define LCD_SCREEN_WIDTH_PX 84
#define LCD_SCREEN_HEIGHT_PX 48
#define TETRIS_BOARD_COLUMNS 10
#define TETRIS_BOARD_ROWS 15
#define BOX_HEIGHT 3
#define BOX_WIDTH BOX_HEIGHT
#define MAGIC_TETRIS_NUMBER 4
#define NUM_TETRIS_PIECES 7
#include "LcdGraphics.h"

typedef struct{
	//u8** Rotations;
	u8* OccupiedIndiciesForRotations;
	u8 CurrentRotation;
	i8 TopLeftX;
	i8 TopLeftY;
}TetrisPiece;

typedef enum {
	EMPTY,
	FULL
}BOARD_CELL_STATE;

static u16 _gameBoardArray[TETRIS_BOARD_ROWS];

/*
 * external dependencies
 * that are passed in to
 * Tetris_Init
 * */
static TetrisDrawRect _drawRect;
static TetrisDrawActiveBlock _drawActiveBlock;
static TetrisDrawStationaryBlock _drawStationaryBlock;
static TetrisGetNextBlockIndex _getNextBlockIndex;
static TetrisLinesClearedCallback _onLinesCleared;
static TetrisGameOverCallback _onGameOver;



static TetrisPiece _currentPiece;
static TetrisPiece _nextPiece;
static u8 _GameBoardXOffset = 5;
static u8 _GameBoardYOffset = 0;

/*
 * On a 4 * 4 matrix with the tetris block drawn in it according to the diagram
 * on this page https://tetris.fandom.com/wiki/SRS, which four indicies have a block
 * for each of the four rotations with zero being the top left corner
 * */
static const u8 _IPieceOccupiedIndices[MAGIC_TETRIS_NUMBER][MAGIC_TETRIS_NUMBER] = {
		{4,5,6,7},
		{2,6,10,14},
		{8,9,10,11},
		{1,5,9,13}

};

static const u8 _JPieceOccupiedIndicies[MAGIC_TETRIS_NUMBER][MAGIC_TETRIS_NUMBER] = {
		{0,4,5,6},
		{1,2,5,9},
		{4,5,6,10},
		{1,5,8,9}
};

static const u8 _TPieceOccupiedIndicies[MAGIC_TETRIS_NUMBER][MAGIC_TETRIS_NUMBER] = {
		{5,1,4,6},
		{5,1,9,6},
		{5,4,9,6},
		{5,4,9,1},

};

static const u8 _LPieceOccupiedIndicies[MAGIC_TETRIS_NUMBER][MAGIC_TETRIS_NUMBER] = {
		{2,4,5,6},
		{1,5,9,10},
		{4,5,6,8},
		{0,1,5,9}
};

static const u8 _OPieceOccupiedIndicies[MAGIC_TETRIS_NUMBER][MAGIC_TETRIS_NUMBER] = {
		{1,2,5,6},
		{1,2,5,6},
		{1,2,5,6},
		{1,2,5,6},
};

static const u8 _SPieceOccupiedIndicies[MAGIC_TETRIS_NUMBER][MAGIC_TETRIS_NUMBER] = {
		{1,2,4,5},
		{1,5,6,10},
		{5,6,8,9},
		{0,4,5,9},
};


static const u8 _ZPieceOccupiedIndicies[MAGIC_TETRIS_NUMBER][MAGIC_TETRIS_NUMBER] = {
		{0,1,5,6},
		{2,5,6,9},
		{4,5,9,10},
		{1,4,5,8},

};


static u8* _TetrisPieces[NUM_TETRIS_PIECES]  = {_ZPieceOccupiedIndicies, _SPieceOccupiedIndicies,_OPieceOccupiedIndicies,_LPieceOccupiedIndicies,_TPieceOccupiedIndicies,_JPieceOccupiedIndicies,_IPieceOccupiedIndices, };




static BOARD_CELL_STATE GetStateAtXY(u8 x, u8 y){
	return (_gameBoardArray[y] & (1 << x)) == 0 ? EMPTY : FULL;
}

static void SetStateAtXY(u8 x, u8 y, BOARD_CELL_STATE newCellState){
	if(newCellState == FULL){
		_gameBoardArray[y] |= (1 << x);
	}
	else{
		_gameBoardArray[y] &= ~(1 << x);
	}
}




static void Tetris_DrawCurentPiece(){
	u8* thisRotationIndicies = _currentPiece.OccupiedIndiciesForRotations + (_currentPiece.CurrentRotation * MAGIC_TETRIS_NUMBER);
	for(int i=0;i<MAGIC_TETRIS_NUMBER; i++){
		u8 thisOffsetIndex = thisRotationIndicies[i];
		u8 thisSquareXOffsetFromTL = (thisOffsetIndex % MAGIC_TETRIS_NUMBER) * BOX_WIDTH;
		u8 thisSquareYOffsetFromTL = (thisOffsetIndex / MAGIC_TETRIS_NUMBER) * BOX_HEIGHT;
		i8 xCoordToDrawAt = (_currentPiece.TopLeftX * BOX_WIDTH) + thisSquareXOffsetFromTL;
		i8 yCoordToDrawAt = (_currentPiece.TopLeftY * BOX_HEIGHT) + thisSquareYOffsetFromTL;

		if(yCoordToDrawAt >= 0){
			_drawActiveBlock(
					xCoordToDrawAt + _GameBoardXOffset,
					yCoordToDrawAt,
					xCoordToDrawAt + BOX_WIDTH + _GameBoardXOffset,
					yCoordToDrawAt + BOX_HEIGHT
			);
		}

	}
}

static void Tetris_DrawNextPiece(){
	u8* thisRotationIndicies = _nextPiece.OccupiedIndiciesForRotations + (_nextPiece.CurrentRotation * MAGIC_TETRIS_NUMBER);
	for(int i=0;i<MAGIC_TETRIS_NUMBER; i++){
		u8 thisOffsetIndex = thisRotationIndicies[i];
		u8 thisSquareXOffsetFromTL = (thisOffsetIndex % MAGIC_TETRIS_NUMBER) * BOX_WIDTH;
		u8 thisSquareYOffsetFromTL = (thisOffsetIndex / MAGIC_TETRIS_NUMBER) * BOX_HEIGHT;
		i8 xCoordToDrawAt = ((TETRIS_BOARD_COLUMNS+1) * BOX_WIDTH) + thisSquareXOffsetFromTL;
		i8 yCoordToDrawAt = (0 * BOX_HEIGHT) + thisSquareYOffsetFromTL;
		if(yCoordToDrawAt >= 0){
			_drawActiveBlock(
					xCoordToDrawAt + _GameBoardXOffset,
					yCoordToDrawAt,
					xCoordToDrawAt + BOX_WIDTH + _GameBoardXOffset,
					yCoordToDrawAt + BOX_HEIGHT
			);
		}
	}
}

void Tetris_DrawTetrisBoard(bool drawNextPiece){
	_drawRect(
			_GameBoardXOffset,
			_GameBoardYOffset,
			_GameBoardXOffset + BOX_WIDTH * TETRIS_BOARD_COLUMNS,
			_GameBoardYOffset + BOX_WIDTH * TETRIS_BOARD_ROWS);
	for(int row=0; row<TETRIS_BOARD_ROWS; row++){
		for(int col=0; col<TETRIS_BOARD_COLUMNS; col++){
			if(GetStateAtXY(col,row) == FULL){

				u8 tlX = _GameBoardXOffset + col * BOX_WIDTH;
				u8 tlY = _GameBoardYOffset + row * BOX_HEIGHT;
				u8 brX = _GameBoardXOffset + (col * BOX_WIDTH) + BOX_WIDTH;
				u8 brY = _GameBoardYOffset + (row * BOX_HEIGHT) + BOX_HEIGHT;

				_drawStationaryBlock(tlX,tlY,brX,brY);
			}
		}
	}
	Tetris_DrawCurentPiece();
	if(drawNextPiece == true){
		Tetris_DrawNextPiece();
	}
}

static void InitNewCurrentPiece(){
	_currentPiece.TopLeftX = 2;
	_currentPiece.TopLeftY = -2;
	_currentPiece.OccupiedIndiciesForRotations = _TetrisPieces[_getNextBlockIndex()];
	_currentPiece.CurrentRotation = 0;
}

static void InitNewNextPiece(){
	_nextPiece.TopLeftX = 2;
	_nextPiece.TopLeftY = -2;
	_nextPiece.OccupiedIndiciesForRotations = _TetrisPieces[_getNextBlockIndex()];
	_nextPiece.CurrentRotation = 0;
}

static void CopyNextPieceToCurrent(){
	memcpy(&_currentPiece, &_nextPiece, sizeof(TetrisPiece));
}

void Tetris_ResetTetrisBoard(){
	memset(_gameBoardArray,EMPTY,sizeof(_gameBoardArray));
	InitNewCurrentPiece();
	InitNewNextPiece();
}
void Tetris_Init(
		TetrisDrawRect drawRect,
		TetrisDrawStationaryBlock drawStationaryBlock,
		TetrisDrawActiveBlock drawActiveBlock,
		TetrisGetNextBlockIndex getNextBlockIndex,
		TetrisLinesClearedCallback onLinesCleared,
		TetrisGameOverCallback onGameOver){
	_drawRect = drawRect;
	_drawStationaryBlock =  drawStationaryBlock;
	_drawActiveBlock = drawActiveBlock;
	_getNextBlockIndex = getNextBlockIndex;
	_onLinesCleared = onLinesCleared;
	_onGameOver = onGameOver;
	//Tetris_ResetTetrisBoard();
}

static bool IsBlockAllowedToBeHere(i8 x, i8 y){
	if((y < 0) && (x>=0 && x < TETRIS_BOARD_COLUMNS)){
		return true;
	}
	if(GetStateAtXY(x, y) == FULL){
		return false;
	}
	if(x >= TETRIS_BOARD_COLUMNS || y >= TETRIS_BOARD_ROWS){
		return false;
	}
	if(x < 0){
		return false;
	}
	return true;
}

static bool WriteCurrentPieceToGameBoardArray(){
	u8* thisRotationIndicies = _currentPiece.OccupiedIndiciesForRotations + (_currentPiece.CurrentRotation * MAGIC_TETRIS_NUMBER);
	for(int i=0; i< MAGIC_TETRIS_NUMBER; i++){
		u8 thisOffsetIndex = thisRotationIndicies[i];
		u8 thisSquareXOffsetFromTL = (thisOffsetIndex % MAGIC_TETRIS_NUMBER);
		u8 thisSquareYOffsetFromTL = (thisOffsetIndex / MAGIC_TETRIS_NUMBER);
		i8 xCoord = (_currentPiece.TopLeftX) + thisSquareXOffsetFromTL;
		i8 yCoord = (_currentPiece.TopLeftY) + thisSquareYOffsetFromTL;
		if(yCoord < 0){
			return true;
		}
		if(yCoord >= 0){
			SetStateAtXY(xCoord,yCoord,FULL);
		}
	}
	return false;
}

static u8 IsMoveValid(){
	u8* thisRotationIndicies = _currentPiece.OccupiedIndiciesForRotations + (_currentPiece.CurrentRotation * MAGIC_TETRIS_NUMBER);
	for(int i=0; i< MAGIC_TETRIS_NUMBER; i++){
		u8 thisOffsetIndex = thisRotationIndicies[i];
		u8 thisSquareXOffsetFromTL = (thisOffsetIndex % MAGIC_TETRIS_NUMBER);
		u8 thisSquareYOffsetFromTL = (thisOffsetIndex / MAGIC_TETRIS_NUMBER);
		i8 xCoord = (_currentPiece.TopLeftX) + thisSquareXOffsetFromTL;
		i8 yCoord = (_currentPiece.TopLeftY) + thisSquareYOffsetFromTL;
		if(IsBlockAllowedToBeHere(xCoord,yCoord) == false){
			return 0;
		}
	}
	return 1;
}

static u8 CheckForAndClearLines(){
	u8 linesToClear = 0;
	bool lastLineWasCleared = false;
	for(int i=TETRIS_BOARD_ROWS-1; i>=0; i--){
		if(_gameBoardArray[i] == 1023){
			linesToClear++;
			lastLineWasCleared = true;
		}
		else{
			lastLineWasCleared = false;
		}

		if(linesToClear > 0 && lastLineWasCleared == false){
			_gameBoardArray[i+linesToClear] = _gameBoardArray[i];
			_gameBoardArray[i] = 0;
		}

	}
	return linesToClear;
}

void Tetris_Rotate(){
	_currentPiece.CurrentRotation++;
	if(_currentPiece.CurrentRotation >= MAGIC_TETRIS_NUMBER){
		_currentPiece.CurrentRotation = 0;
	}
	if(IsMoveValid() == 0){
		if(_currentPiece.CurrentRotation == 0){
			_currentPiece.CurrentRotation = MAGIC_TETRIS_NUMBER - 1;
		}
		else{
			_currentPiece.CurrentRotation--;
		}
	}
}

MoveDownResult Tetris_MoveDown(){
	_currentPiece.TopLeftY++;
	if(IsMoveValid() == 0){
		_currentPiece.TopLeftY--;
		bool hasGameOverOccured = WriteCurrentPieceToGameBoardArray();
		u8 linesCleared = CheckForAndClearLines();
		CopyNextPieceToCurrent();
		InitNewNextPiece();
		if(hasGameOverOccured == true){
			_onGameOver();
			Tetris_ResetTetrisBoard();
			return MoveDownResultGameOver;
		}
		if(linesCleared > 0){
			_onLinesCleared(linesCleared);
		}
		return Settled;

	}
	return StillMoving;
}

void Tetris_MoveLeft(){
	_currentPiece.TopLeftX--;
	if(IsMoveValid() == 0){
		_currentPiece.TopLeftX++;
	}
}

void Tetris_MoveRight(){
	_currentPiece.TopLeftX++;
	if(IsMoveValid() == 0){
		_currentPiece.TopLeftX--;
	}
}

void Tetris_MoveUp(){
	_currentPiece.TopLeftY--;
	if(IsMoveValid() == 0){
		_currentPiece.TopLeftY++;
	}
}
















