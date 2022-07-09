/*
 * TetrisPlayAgainScreen.c
 *
 *  Created on: Jul 6, 2022
 *      Author: James.Marshall
 */
#include <TetrisHighLevelModesStateMachineDefs.h>
#include "main.h"
#include "LcdGraphics.h"
extern  ADC_HandleTypeDef hadc2;
extern LCD_PCD8544_screen_t gLcdScreen;

static u8 _startLevel;

Tetris_Modes_StateTriggers TetrisPlayAgain_Update(u32 TimePassed){

	/* Will have option to go back to main menu.  67 */
	GPIO_PinState buttonBState = HAL_GPIO_ReadPin(BUTTON_B_GPIO_Port, BUTTON_B_Pin);
	if(buttonBState == GPIO_PIN_SET){
		TetrisMain_SetStateMachineDataPointer(&_startLevel);
		return StartPlaying;
	}

	return NoChange;
}

void TetrisPlayAgain_Init(){

}

void TetrisPlayAgain_OnEnter(void* stateMachineDataPtr, Tetris_Modes_States previousState){
	_startLevel = *((u8*)stateMachineDataPtr);
	gfxClearFrameBuffer();
	ClearUpdateRegions();
	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(0,0,0,84);
	gfxWriteTextLineToFrameBuffer(0,0,"Play Again?");
	gfxFinishDrawing(&gLcdScreen);

}

void TetrisPlayAgain_OnExit(void* stateMachineDataPtr, Tetris_Modes_States nextState){
	ClearScreen(&gLcdScreen);
}
