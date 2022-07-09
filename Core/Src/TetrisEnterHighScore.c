/*
 * TetrisEnterHighScore.c
 *
 *  Created on: Jul 9, 2022
 *      Author: James.Marshall
 */
#include <stdio.h>

#include "TetrisEnterHighScore.h"
#include "TetrisHighScores.h"
#include "main.h"
#include "LcdGraphics.h"
extern  ADC_HandleTypeDef hadc2;
extern LCD_PCD8544_screen_t gLcdScreen;

static u8 _startLevel;

void DrawHighScoresToFrameBuffer(){
	char highScoreLineBuffer[10];
	for(u32 i=0; i < NUM_HIGHSCORES_SAVED; i++){
		const HighScore* highScore = TetrisHighScores_GetHighScoreAtIndex(i);
		sprintf(highScoreLineBuffer,"%c%c%c %lu",
				highScore->Initials[0],
				highScore->Initials[1],
				highScore->Initials[2],
				highScore->Score);
		gfxWriteTextLineToFrameBuffer(i+1,0,highScoreLineBuffer);

	}
}

Tetris_Modes_StateTriggers TetrisEnterHighScore_Update(u32 TimePassed){

	/* Will have option to go back to main menu.  67 */
	GPIO_PinState buttonBState = HAL_GPIO_ReadPin(BUTTON_B_GPIO_Port, BUTTON_B_Pin);
	if(buttonBState == GPIO_PIN_SET){
		TetrisMain_SetStateMachineDataPointer(&_startLevel);
		return HighScoreEntered;
	}

	return NoChange;
}

void TetrisEnterHighScore_OnEnter(void* stateMachineDataPtr, Tetris_Modes_States previousState){
	_startLevel = *((u8*)stateMachineDataPtr);
	gfxClearFrameBuffer();
	ClearUpdateRegions();
	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(0,0,0,84);
	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(1,1,0,84);
	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(2,2,0,84);
	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(3,3,0,84);
	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(4,4,0,84);
	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(5,5,0,84);


	gfxWriteTextLineToFrameBuffer(0,0,"High Score");
	DrawHighScoresToFrameBuffer();
	gfxFinishDrawing(&gLcdScreen);

}

void TetrisEnterHighScore_OnExit(void* stateMachineDataPtr, Tetris_Modes_States nextState){
	ClearScreen(&gLcdScreen);
}
