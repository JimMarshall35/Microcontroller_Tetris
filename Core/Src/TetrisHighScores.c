/*
 * TetrisFlashMemoryHighScores.c
 *
 *  Created on: Jul 9, 2022
 *      Author: James.Marshall
 */
#include <TetrisHighScores.h>
#include "main.h"
#include "BasicTypedefs.h"
#include "stm32f3xx_hal_flash_ex.h"


static HighScore _highScores[NUM_HIGHSCORES_SAVED] = {
		{{'J','i','m'}, 200420},
		{{'J','i','m'}, 100000},
		{{'F','A','T'}, 50000},
		{{'J','i','m'}, 25000},
		{{'J','i','m'}, 5000},
};
static u32 _highScoreCellSize = 0;

static void SetHighScoreCellSize(){
	u32 size = sizeof(HighScore);
	_highScoreCellSize = size;

	while(_highScoreCellSize % 4 != 0)
		_highScoreCellSize++;

}
static void LoadHighScores(HighScore* output, u32 startAddress){

	for(u32 i=0; i<NUM_HIGHSCORES_SAVED; i++){
		HighScore* hs = startAddress + (i * _highScoreCellSize);
		output[i].Initials[0] = hs->Initials[0];
		output[i].Initials[1] = hs->Initials[1];
		output[i].Initials[2] = hs->Initials[2];
		output[i].Score = hs->Score;
	}

}

static void SaveSingleHighScore(HighScore* input, u32 address){
	for(int i=0; i<_highScoreCellSize/4; i++){
		u32 wordToWrite = *((u32*)input + i);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,address + (i*4), wordToWrite);
	}
}

static void SaveHighScores(HighScore* input, u32 addressToWrite){

	for(u32 i=0; i<NUM_HIGHSCORES_SAVED; i++){
		SaveSingleHighScore(&input[i], addressToWrite);
		addressToWrite += _highScoreCellSize;
	}

}

u32 TetrisHighScores_Load(u32 startAddress){
	/*
		 * used to initially bootstrap the high scores into rom

		HighScore hs[NUM_HIGHSCORES_SAVED] = {
				{{'J','i','m'}, 200420},
				{{'J','i','m'}, 100000},
				{{'F','A','T'}, 50000},
				{{'J','i','m'}, 25000},
				{{'J','i','m'}, 5000},
		};
	SaveHighScores(hs);
	*/
	SetHighScoreCellSize();
	LoadHighScores(_highScores, startAddress);
	return startAddress + (_highScoreCellSize * NUM_HIGHSCORES_SAVED);
}


bool TetrisHighScores_IsNewHighScore(u32 potentialHighScore){
	for(u32 i=0; i<NUM_HIGHSCORES_SAVED; i++){
		if(potentialHighScore > _highScores[i].Score){
			return true;
		}
	}
	return false;
}

HighScore* TetrisHighScores_GetHighScoreAtIndex(u8 atIndex){
	return &_highScores[atIndex];
}


u32 TetrisHighScores_Save(u32 startAddress){
	SetHighScoreCellSize();

	SaveHighScores(_highScores, startAddress);
	return startAddress + (_highScoreCellSize * NUM_HIGHSCORES_SAVED);
}

















