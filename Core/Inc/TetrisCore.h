/*
 * TetrisCore.h
 *
 *  Created on: Jun 27, 2022
 *      Author: James.Marshall
 *
 * The portable core of tetris.
 * Bring your own graphics and rng routines
 * and implement the game of tetris in your own
 * game loop.
 *
 */

#ifndef INC_TETRISCORE_H_
#define INC_TETRISCORE_H_

#include "BasicTypedefs.h"

typedef enum{
	None,
	StillMoving,
	Settled,
	MoveDownResultGameOver
}MoveDownResult;
/*
 * Draw one of the blocks that has settled down
 * on the bottom of the tetris board
 */
typedef void (*TetrisDrawStationaryBlock)(u8 tlX, u8 tlY, u8 brX, u8 brY);

/*
 * Draw one of the blocks that make up the moving piece,
 * specifiying coords of topLeft and bottom right in pixels
 * */
typedef void (*TetrisDrawActiveBlock)(u8 tlX, u8 tlY, u8 brX, u8 brY);

/*
 * Used to draw a rectangle around the board
 * */
typedef void (*TetrisDrawRect)(u8 x0, u8 y0, u8 x1, u8 y1);

/*
 * must return a number between 0 and 7
 * used to set the next tetris piece so
 * preferably a random one.
 * */
typedef u8 (*TetrisGetNextBlockIndex)(void);

/*
 * Callback for when lines are cleared
 * */
typedef void(*TetrisLinesClearedCallback)(u8 howManyLines);

/*
 * Callback for when it's game over
 */
typedef void(*TetrisGameOverCallback)(void);

/*
 * Draw the whole tetris board including the current piece
 * */
void Tetris_DrawTetrisBoard(bool drawNextPiece);

void Tetris_ResetTetrisBoard();
/*
 * initiate the tetris core code module passing in
 * dependencies in the form of function pointers
 * */
void Tetris_Init(
		TetrisDrawRect drawRect,
		TetrisDrawStationaryBlock drawStationaryBlock,
		TetrisDrawActiveBlock drawActiveBlock,
		TetrisGetNextBlockIndex getNextBlockIndex,
		TetrisLinesClearedCallback onLinesCleared,
		TetrisGameOverCallback onGameOver);

/*
 * Rotate the tetris piece
 * */
void Tetris_Rotate();
/*
 * Move piece left and right
 * */
void Tetris_MoveLeft();
void Tetris_MoveRight();
/*
 * Call this regularly to cause the current tetromino to move downwards.
 * Cause it to be called more regularly upon some input so the player can
 * "push pieces down".
 *
 * returns: whether the piece has settled
 * */
MoveDownResult Tetris_MoveDown();
/* for testing */
void Tetris_MoveUp();

#endif /* INC_TETRISCORE_H_ */
