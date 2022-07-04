/*
 * TetrisGame.c
 *
 *  Created on: Jun 29, 2022
 *      Author: James.Marshall
 *
 * A main loop that calls the functions in TetrisCore.h
 * - an implementation of tetris using TetrisCore
 */
#include <stdlib.h>
#include <stdio.h>

#include "TetrisCore.h"
#include "AnalogStick.h"
#include "BasicTypedefs.h"
#include "LcdGraphics.h"
#include "main.h"

//#include "stm32f3xx_hal_flash.h"
//__attribute__((__section__(".user_data"))) const char userConfig[64];



extern  ADC_HandleTypeDef hadc2;
extern LCD_PCD8544_screen_t gLcdScreen;
#define TETRIS_BOARD_RIGHT_EDGE_COL 37
#define TETRIS_BOARD_LEFT_EDGE_COL 4
#define TETRIS_SCORE_SINGLE 40
#define TETRIS_SCORE_DOUBLE 100
#define TETRIS_SCORE_TRIPLE 300
#define TETRIS_SCORE_TETRIS 1200

u32 _tetrisTimer = 0;
u32 _autoMoveInterval = 500;
GPIO_PinState _lastButtonBState = GPIO_PIN_RESET;
u16 _linesCleared = 0;
u32 _score = 0;
bool _newScoreToDisplay = true;
MoveDownResult _movingDownResult = Settled;

//u32 FlashAddress = 0x08040000;


void Write_Flash(u8 data, u8 index)
{
}


u8 GetRandomNumberBetweenZeroAndSix(){
	return (u8)rand() % 7;
}


void DrawStationaryBlock(u8 tlX, u8 tlY, u8 brX, u8 brY){
	gfxDrawAxisAlignedRect(tlX, tlY, brX, brY);
	gfxDrawLine(tlX, tlY, brX, brY);
}

void OnLinesCleared(u8 numLinesCleared){
	_linesCleared += numLinesCleared;
	switch(numLinesCleared){
	case 1:
		_score += TETRIS_SCORE_SINGLE;
		break;
	case 2:
		_score += TETRIS_SCORE_DOUBLE;
		break;
	case 3:
		_score += TETRIS_SCORE_TRIPLE;
		break;
	case 4:
		_score += TETRIS_SCORE_TETRIS;
		break;
	}
	_newScoreToDisplay = true;
}

void OnGameOver(){
	_linesCleared = 0;
	_score = 0;
	_newScoreToDisplay = true;
}

void IncrementTetrisTimer(u32 timePassed, bool moveDown){
	_tetrisTimer += timePassed;
	if(_tetrisTimer > _autoMoveInterval){
		if(moveDown == true){
			_movingDownResult = Tetris_MoveDown();
		}
		_tetrisTimer = 0;
	}
}

void WriteScoreToFrameBuffer(){
	u8 scoreNumBuffer[7];
	sprintf(scoreNumBuffer, "%lu",_score);
	gfxWriteTextLineToFrameBuffer(1,TETRIS_BOARD_RIGHT_EDGE_COL + 1,scoreNumBuffer);

}

void TetrisGame(u32 timePassed){

	const i32 twelveBitMax = 4096;
	u16 x, y;
	ReadAnalogStick(&hadc2,&x,&y);
	i32 analogXChange = (x - (twelveBitMax/2));
	i32 analogYChange = (y - (twelveBitMax/2));
	bool movingDown = true;
	GPIO_PinState buttonBState = HAL_GPIO_ReadPin(BUTTON_B_GPIO_Port, BUTTON_B_Pin);
	if(analogXChange > (twelveBitMax/3)){
		Tetris_MoveRight();
	}
	else if(analogXChange < -(twelveBitMax/3)){
		Tetris_MoveLeft();
	}
	else if(analogYChange > (twelveBitMax/3)){
		_movingDownResult = Tetris_MoveDown();
		movingDown = false;
	}
	if(buttonBState == GPIO_PIN_SET && _lastButtonBState == GPIO_PIN_RESET){
		Tetris_Rotate();
	}

	_lastButtonBState = buttonBState;

	IncrementTetrisTimer(timePassed, movingDown);

	gfxClearFrameBuffer();


	Tetris_DrawTetrisBoard((_movingDownResult == Settled || _movingDownResult == GameOver) ? true : false);

	WriteScoreToFrameBuffer();


	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(0,5,TETRIS_BOARD_LEFT_EDGE_COL,TETRIS_BOARD_RIGHT_EDGE_COL);


	if(_movingDownResult == Settled || _movingDownResult == GameOver){
		/* update the area where the next block is shown */
		UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(0,0,0,53);
	}
	if(_newScoreToDisplay == true){
		/* Update the new area of the screen with the new score.
		 * TODO: get actual length of score string instead of using 84 */
		UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(1,1,0,84);
		_newScoreToDisplay = false;
	}
	gfxFinishDrawing(&gLcdScreen);

}

void TetrisGame_Init(){
	Tetris_Init(
			&gfxDrawAxisAlignedRect,
			&DrawStationaryBlock,
			&gfxDrawAxisAlignedRect,
			&GetRandomNumberBetweenZeroAndSix,
			&OnLinesCleared,
			&OnGameOver);

}














