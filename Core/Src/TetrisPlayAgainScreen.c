/*
 * TetrisPlayAgainScreen.c
 *
 *  Created on: Jul 6, 2022
 *      Author: James.Marshall
 */
#include "TetrisHighLevelModesStateMachine.h"
#include "main.h"
#include "LcdGraphics.h"
extern  ADC_HandleTypeDef hadc2;
extern LCD_PCD8544_screen_t gLcdScreen;

static u8 _startLevel;

Tetris_Modes_StateTriggers TetrisPlayAgain_Update(u32 TimePassed){

	/* Will have option to go back to main menu. */
	GPIO_PinState buttonBState = HAL_GPIO_ReadPin(BUTTON_B_GPIO_Port, BUTTON_B_Pin);
	if(buttonBState == GPIO_PIN_SET){
		TetrisMain_SetStateMachineDataPointer(&_startLevel);
		return StartPlaying;
	}

	return NoChange;
}

void TetrisPlayAgain_Init(){

}

void TetrisPlayAgain_OnEnter(void* stateMachineDataPtr){
	_startLevel = *((u8*)stateMachineDataPtr);
	gfxClearFrameBuffer();
	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(0,0,0,84);
	gfxWriteTextLineToFrameBuffer(2,0,"Play Again?");
	gfxFinishDrawing(&gLcdScreen);

}
