/*
 * TetrisLevelSelectScreen.c
 *
 *  Created on: Jul 6, 2022
 *      Author: James.Marshall
 */


#include "TetrisLevelSelectScreen.h"
#include "LcdGraphics.h"
#include "main.h"
#include "AnalogStick.h"

extern  ADC_HandleTypeDef hadc2;
extern LCD_PCD8544_screen_t gLcdScreen;
static i8 _startLevel = 3;
static bool _isAnalogStickExtended = false;

u32 randomSeed = 0;

void WriteStartLevelToFrameBuffer(){
	u8 stringBuffer[11];
	sprintf(stringBuffer,"Level: %lu",_startLevel);
	gfxClearFrameBufferRow(2,0,LCD_PCD8544_CHAR_WIDTH*11);
	gfxWriteTextLineToFrameBuffer(2,0,stringBuffer);
}

void ChangeCurrentLevelUpOrDown(i8 incOrDec){
	_startLevel += incOrDec;
	if(incOrDec < 0){
		if(_startLevel < 0){
			_startLevel = 15;
		}
	}
	else if(incOrDec > 0){
		if(_startLevel > 15){
			_startLevel = 0;
		}
	}
	else{

	}
}

Tetris_Modes_StateTriggers TetrisLevelSelect_Update(u32 TimePassed){
	randomSeed += TimePassed;
	i32 analogYChange, analogXChange;
	ANALOG_STICK_DPAD_RESULT res = EmulateDPadReturningXAndYChange(&hadc2,&analogXChange,&analogYChange);
	switch(res){
	case UP:
		ChangeCurrentLevelUpOrDown(1);
		randomSeed += analogYChange;
		break;
	case DOWN:
		ChangeCurrentLevelUpOrDown(-1);
		randomSeed += analogYChange;
		break;
	}

	GPIO_PinState buttonBState = HAL_GPIO_ReadPin(BUTTON_B_GPIO_Port, BUTTON_B_Pin);
	if(buttonBState == GPIO_PIN_SET){
		TetrisMain_SetStateMachineDataPointer(&_startLevel);
		return StartPlaying;
	}

	WriteStartLevelToFrameBuffer();
	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(2,2,0,84);
	gfxFinishDrawing(&gLcdScreen);



	return NoChange;
}

void TetrisLevelSelect_Init(){

}

void TetrisLevelSelect_OnEnter(void* stateMachineDataPtr, Tetris_Modes_States previousState){
	gfxWriteTextLineToFrameBuffer(0,0,"Select Level");
	gfxClearFrameBuffer();
	ClearUpdateRegions();
	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(0,0,0,84);
	gfxFinishDrawing(&gLcdScreen);
}

void TetrisLevelSelect_OnExit(void* stateMachineDataPtr, Tetris_Modes_States nextState){
	srand(randomSeed);
}
