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
#include "MusicPlayer.h"

typedef enum{
	SelectLevel = 0,
	MusicOnOff,
	NumMenuRows
}MenuRows;

#define TEST_TUNE_LENGTH 55
#define TEMPO_MODIFIER 3
MusicNote tetrisTune[TEST_TUNE_LENGTH] = {
   {E4,1043/TEMPO_MODIFIER},
   {B3,474/TEMPO_MODIFIER},
   {C4,479/TEMPO_MODIFIER},
   {D4,1014/TEMPO_MODIFIER},
   {C4,474/TEMPO_MODIFIER},
   {B3,476/TEMPO_MODIFIER},
   {A3,1049/TEMPO_MODIFIER},
   {A3,474/TEMPO_MODIFIER},
   {C4,479/TEMPO_MODIFIER},
   {E4,1014/TEMPO_MODIFIER},
   {D4,474/TEMPO_MODIFIER},
   {C4,476/TEMPO_MODIFIER},
   {B3,1049/TEMPO_MODIFIER},
   {B3,453/TEMPO_MODIFIER},
   {C4,437/TEMPO_MODIFIER},
   {D4,1014/TEMPO_MODIFIER},
   {E4,1014/TEMPO_MODIFIER},
   {C4,1049/TEMPO_MODIFIER},
   {A3,1014/TEMPO_MODIFIER},
   {A3,1014/TEMPO_MODIFIER},

   {BUZZER_OFF,1010/TEMPO_MODIFIER},
   {D4,1014/TEMPO_MODIFIER},
   {F4,479/TEMPO_MODIFIER},
   {A4,979/TEMPO_MODIFIER},
   {G4,510/TEMPO_MODIFIER},
   {F4,478/TEMPO_MODIFIER},
   {E4,1573/TEMPO_MODIFIER},
   {C4,443/TEMPO_MODIFIER},
   {E4,961/TEMPO_MODIFIER},
   {D4,483/TEMPO_MODIFIER},
   {C4,479/TEMPO_MODIFIER},
   {B3,1515/TEMPO_MODIFIER},
   {C4,478/TEMPO_MODIFIER},
   {D4,1018/TEMPO_MODIFIER},
   {E4,1014/TEMPO_MODIFIER},
   {C4,1014/TEMPO_MODIFIER},
   {A3,1011/TEMPO_MODIFIER},
   {A3,1049/TEMPO_MODIFIER},

   {BUZZER_OFF,1614/TEMPO_MODIFIER},
   {E4,2062/TEMPO_MODIFIER},
   {C4,2026/TEMPO_MODIFIER},
   {D4,2062/TEMPO_MODIFIER},
   {B3,2026/TEMPO_MODIFIER},
   {C4,2062/TEMPO_MODIFIER},
   {A3,2026/TEMPO_MODIFIER},
   {GSHARP3,2062/TEMPO_MODIFIER},
   {B3,2026/TEMPO_MODIFIER},
   {E4,2062/TEMPO_MODIFIER},
   {C4,2026/TEMPO_MODIFIER},
   {D4,2062/TEMPO_MODIFIER},
   {B3,2026/TEMPO_MODIFIER},
   {C4,1048/TEMPO_MODIFIER},
   {E4,1014/TEMPO_MODIFIER},
   {A4,2026/TEMPO_MODIFIER},
   {GSHARP4,4090/TEMPO_MODIFIER},

};

extern  ADC_HandleTypeDef hadc2;
extern LCD_PCD8544_screen_t gLcdScreen;
static i8 _startLevel = 3;
static bool _isAnalogStickExtended = false;
static PlayerState _musicState = PLAYING;

static u32 _randomSeed = 0;
static u32 _blinkTimer = 0;

#define CHANGE_BLINK_STATE_AFTER_MS 500

static const char* _selectLevelStartString_WithArrow    = "<  ";
static const char* _selectLevelStartString_WithoutArrow = "   ";


static const char* _selectLevelEndString_OneDigit_withArrow = "  >";
static const char* _selectLevelEndString_TwoDigit_withArrow = " >";

static const char* _selectLevelEndString_OneDigit_withoutArrow = "   ";
static const char* _selectLevelEndString_TwoDigit_withoutArrow = "  ";


static const char* _musicOnOffStartString_WithArrow    = "< ";
static const char* _musicOnOffStartString_WithoutArrow = "  ";

static const char* _musicOnOffEndString_withArrow = " >";
static const char* _musicOnOffEndString_withoutArrow = "  ";


static bool _blinkState = true;

static int _selectedRow = SelectLevel;

static bool _buttonReleasedSinceEnteringState = false;

static u8 title[504] = { 255, 255, 3, 3, 243, 243, 3, 3, 255, 255, 3, 3, 243, 243, 3, 3, 255, 255, 3, 3, 255, 255, 3, 3, 243, 243, 255, 255, 3, 3, 243, 243, 51, 51, 51, 51, 51, 51, 255, 255, 12, 12, 12, 140, 252, 252, 12, 204, 236, 108, 108, 104, 224, 192, 0, 128, 192, 192, 192, 224, 240, 248, 152, 0, 0, 128, 192, 224, 224, 224, 224, 224, 230, 79, 15, 134, 192, 224, 224, 224, 224, 224, 224, 224, 255, 255, 192, 192, 255, 255, 192, 192, 207, 207, 192, 192, 255, 255, 192, 192, 255, 255, 192, 192, 255, 255, 192, 192, 207, 207, 255, 255, 192, 192, 255, 255, 192, 192, 207, 207, 192, 192, 255, 255, 0, 48, 120, 127, 31, 3, 0, 31, 63, 51, 51, 51, 51, 17, 0, 56, 61, 127, 103, 97, 97, 97, 64, 0, 31, 63, 31, 1, 112, 248, 252, 222, 14, 0, 112, 243, 231, 207, 206, 206, 206, 254, 124, 57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
extern unsigned char gFrameBuffer[];


static const char* GetSelectLevelRowStartString(){
	if(_blinkState == false || _selectedRow != SelectLevel){
		return _selectLevelStartString_WithoutArrow;
	}
	else{
		return _selectLevelStartString_WithArrow;
	}
}

static const char* GetSelectLevelRowEndString(){
	if(_blinkState == false || _selectedRow != SelectLevel){
		return _startLevel > 9 ? _selectLevelEndString_TwoDigit_withoutArrow : _selectLevelEndString_OneDigit_withoutArrow;

	}
	else{
		return _startLevel > 9 ? _selectLevelEndString_TwoDigit_withArrow : _selectLevelEndString_OneDigit_withArrow;

	}
}

static const char* GetMusicOnOffRowStartString(){
	if(_blinkState == false || _selectedRow != MusicOnOff){
		return _musicOnOffStartString_WithoutArrow;
	}
	else{
		return _musicOnOffStartString_WithArrow;
	}
}

static const char* GetMusicOnOffEndString(){
	if(_blinkState == false || _selectedRow != MusicOnOff){
		return _musicOnOffEndString_withoutArrow;
	}
	else{
		return _musicOnOffEndString_withArrow;
	}
}

static void WriteStartLevelToFrameBuffer(){
	u8 stringBuffer[15];
	sprintf(stringBuffer,"%sLevel: %lu%s",GetSelectLevelRowStartString(), _startLevel, GetSelectLevelRowEndString());

	gfxClearFrameBufferRow(2,0,LCD_PCD8544_CHAR_WIDTH*14);
	gfxWriteTextLineToFrameBuffer(2,0,stringBuffer);
}

static void WriteMusicOnOffToFrameBuffer(){
	u8 stringBuffer[15];
	sprintf(stringBuffer,"%sMusic: %s%s",GetMusicOnOffRowStartString(),
			_musicState == PLAYING ? "on " : "off",
					GetMusicOnOffEndString());

	gfxClearFrameBufferRow(3,0,LCD_PCD8544_CHAR_WIDTH*14);
	gfxWriteTextLineToFrameBuffer(3,0,stringBuffer);

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

static void IncrementBlinkTimer(u32 TimePassed){
	_blinkTimer += TimePassed;
	if(_blinkTimer >= CHANGE_BLINK_STATE_AFTER_MS){
		TOGGLE_BOOL(_blinkState);
		_blinkTimer = 0;
	}
}

static void TogglePlayerState(){
	switch(_musicState){
	case PLAYING:
		MusicPlayer_PauseTune();
		break;
	case PAUSED:
		MusicPlayer_UnPauseTune();
		break;
	case STOPPED:
		MusicPlayer_StartTune(&tetrisTune[0],TEST_TUNE_LENGTH);
		break;
	}
	_musicState = MusicPlayer_GetState();
}

static void ChangeRowUpOrDown(i8 incOrDec){
	_selectedRow += incOrDec;
	if(_selectedRow >= NumMenuRows){
		_selectedRow = 0;
	}
	else if(_selectedRow < 0){
		_selectedRow = NumMenuRows - 1;
	}
}

static void HandleRowLeft(){
	switch(_selectedRow){
	case SelectLevel:
		ChangeCurrentLevelUpOrDown(-1);
		break;
	case MusicOnOff:
		TogglePlayerState();
		break;
	}
}

static void HandleRowRight(){
	switch(_selectedRow){
	case SelectLevel:
		ChangeCurrentLevelUpOrDown(1);
		break;
	case MusicOnOff:
		TogglePlayerState();
		break;
	}
}

Tetris_Modes_StateTriggers TetrisLevelSelect_Update(u32 TimePassed){
	GPIO_PinState buttonBState = HAL_GPIO_ReadPin(BUTTON_B_GPIO_Port, BUTTON_B_Pin);

	if(_buttonReleasedSinceEnteringState == false){
		if(buttonBState == GPIO_PIN_RESET){
			_buttonReleasedSinceEnteringState = true;
		}
	}
	IncrementBlinkTimer(TimePassed);
	_randomSeed += TimePassed;
	i32 analogYChange, analogXChange;
	ANALOG_STICK_DPAD_RESULT res = EmulateDPadReturningXAndYChange(&hadc2,&analogXChange,&analogYChange);
	switch(res){
	case RIGHT:
		HandleRowRight();
		_randomSeed += analogYChange;
		break;
	case LEFT:
		HandleRowLeft();
		_randomSeed += analogYChange;
		break;
	case UP:
		ChangeRowUpOrDown(1);
		break;
	case DOWN:
		ChangeRowUpOrDown(-1);
		break;
	}



	WriteStartLevelToFrameBuffer();
	WriteMusicOnOffToFrameBuffer();

	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(2,3,0,84);

	gfxFinishDrawing(&gLcdScreen);


	if(buttonBState == GPIO_PIN_SET && (_buttonReleasedSinceEnteringState == true)){
		TetrisMain_SetStateMachineDataPointer(&_startLevel);
		return StartPlaying;
	}

	return NoChange;
}

void TetrisLevelSelect_Init(){

}

void TetrisLevelSelect_OnEnter(void* stateMachineDataPtr, Tetris_Modes_States previousState){
	gfxClearFrameBuffer();
	memcpy(gFrameBuffer,title,504);

	ClearUpdateRegions();
	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(0,2,0,84);
	gfxFinishDrawing(&gLcdScreen);
	_musicState = MusicPlayer_GetState();
	_blinkState = true;
	_buttonReleasedSinceEnteringState =
			HAL_GPIO_ReadPin(BUTTON_B_GPIO_Port, BUTTON_B_Pin) == GPIO_PIN_RESET ? true : false;

}

void TetrisLevelSelect_OnExit(void* stateMachineDataPtr, Tetris_Modes_States nextState){
	ClearScreen(&gLcdScreen);
	srand(_randomSeed);
}
