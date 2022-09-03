/*
 * TetrisPlayAgainScreen.c
 *
 *  Created on: Jul 6, 2022
 *      Author: James.Marshall
 */
#include <TetrisHighLevelModesStateMachineDefs.h>
#include "main.h"
#include "LcdGraphics.h"
#include "AnalogStick.h"

#define CHANGE_BLINK_STATE_AFTER_MS 500
extern  ADC_HandleTypeDef hadc2;
extern LCD_PCD8544_screen_t gLcdScreen;

static u8 _startLevel;
static bool _playAgain = true;
static bool _blinkState = true;
static u32 _blinkTimer = 0;

static const char* StartString_WithArrows = "<    ";
static const char* EndString_WithArrows   = "     >";

static const char* StartString_WithoutArrows = "     ";
static const char* EndString_WithoutArrows   = "      ";

static const char* GetPlayAgainChoiceStartString(){
	return _blinkState == true ? StartString_WithArrows : StartString_WithoutArrows;
}

static const char* GetPlayAgainChoiceEndString(){
	return _blinkState == true ? EndString_WithArrows : EndString_WithoutArrows;
}

static void IncrementBlinkTimer(u32 TimePassed){
	_blinkTimer += TimePassed;
	if(_blinkTimer >= CHANGE_BLINK_STATE_AFTER_MS){
		TOGGLE_BOOL(_blinkState);
		_blinkTimer = 0;
	}
}

static void WritePlayAgainChoicetoFrameBuffer(){
	u8 stringBuffer[15];

	sprintf(stringBuffer,"%s%s%s",
			GetPlayAgainChoiceStartString(),
			_playAgain == true ? "yes" : "no ",
			GetPlayAgainChoiceEndString());

	gfxClearFrameBufferRow(1,0,LCD_PCD8544_CHAR_WIDTH*14);
	gfxWriteTextLineToFrameBuffer(1,0,stringBuffer);

}

Tetris_Modes_StateTriggers TetrisPlayAgain_Update(u32 timePassed){

	/* Will have option to go back to main menu.  67 */
	GPIO_PinState buttonBState = HAL_GPIO_ReadPin(BUTTON_B_GPIO_Port, BUTTON_B_Pin);

	IncrementBlinkTimer(timePassed);

	i32 analogYChange, analogXChange;

	ANALOG_STICK_DPAD_RESULT res = EmulateDPadReturningXAndYChange(&hadc2,&analogXChange,&analogYChange);
	switch(res){
	case RIGHT:
	case LEFT:
		TOGGLE_BOOL(_playAgain);
		break;
	}

	WritePlayAgainChoicetoFrameBuffer();
	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(1,1,0,84);
	gfxFinishDrawing(&gLcdScreen);


	if(buttonBState == GPIO_PIN_SET){
		//TetrisMain_SetStateMachineDataPointer(&_startLevel);
		return _playAgain == true  ? StartPlaying : GoToLevelSelectScreen;
	}

	return NoChange;
}

void TetrisPlayAgain_Init(){

}

void TetrisPlayAgain_OnEnter(void* stateMachineDataPtr, Tetris_Modes_States previousState){
	_startLevel = *((u8*)stateMachineDataPtr);
	_playAgain = true;
	_blinkTimer = 0;
	_blinkState = true;

	gfxClearFrameBuffer();
	ClearUpdateRegions();
	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(0,0,0,84);
	gfxWriteTextLineToFrameBuffer(0,0,"Play Again?");

	gfxFinishDrawing(&gLcdScreen);

}

void TetrisPlayAgain_OnExit(void* stateMachineDataPtr, Tetris_Modes_States nextState){
	ClearScreen(&gLcdScreen);
}
