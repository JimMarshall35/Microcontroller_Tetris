/*
 * TetrisViewHighScores.c
 *
 *  Created on: 3 Sep 2022
 *      Author: James.Marshall
 */


#include "TetrisViewHighScores.h"
#include "TetrisHighScorePrintHelpers.h"
#include "LcdGraphics.h"
#include "main.h"

extern LCD_PCD8544_screen_t gLcdScreen;

static bool _buttonReleasedSinceEnteringState = false;

Tetris_Modes_StateTriggers TetrisViewHighScores_Update(u32 TimePassed){
	GPIO_PinState buttonBState = HAL_GPIO_ReadPin(BUTTON_B_GPIO_Port, BUTTON_B_Pin);

	if(_buttonReleasedSinceEnteringState == false){
		if(buttonBState == GPIO_PIN_RESET){
			_buttonReleasedSinceEnteringState = true;
		}

	}
	if(buttonBState == GPIO_PIN_SET && _buttonReleasedSinceEnteringState == true){
		return GoToLevelSelectScreen;
	}
	return NoChange;
}

void TetrisViewHighScores_OnEnter(void* stateMachineDataPtr, Tetris_Modes_States previousState){
	_buttonReleasedSinceEnteringState =
			HAL_GPIO_ReadPin(BUTTON_B_GPIO_Port, BUTTON_B_Pin) == GPIO_PIN_RESET ? true : false;


	Tetris_DrawHighScoresToFrameBuffer();
	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(0,5,0,84);
	gfxFinishDrawing(&gLcdScreen);
}

void TetrisViewHighScores_OnExit(void* stateMachineDataPtr, Tetris_Modes_States nextState){
	ClearScreen(&gLcdScreen);
}
