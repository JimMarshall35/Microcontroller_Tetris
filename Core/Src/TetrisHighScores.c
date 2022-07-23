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
		{{'c','p','u'}, 40000},
		{{'c','p','u'}, 20000},
		{{'c','p','u'}, 10000},
		{{'c','p','u'}, 5000},
		{{'c','p','u'}, 2500},
};
static u32 _highScoreCellSize = 0;
static u32 _nextHighScoreSaveAddress = 0;

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

static void ErasePagePreservingHighScores(){
	FLASH_EraseInitTypeDef eraseInit;
	eraseInit.NbPages = 1;
	eraseInit.PageAddress = FINAL_PAGE_START_ADDRESS;
	eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;

	u32 pageError;
	HAL_FLASHEx_Erase(&eraseInit,&pageError);
	SaveHighScores(_highScores,FINAL_PAGE_START_ADDRESS);
	TetrisHighScores_Load();
}

static u32 SeekLatestHighScoresStartAddress(){
	if(_highScoreCellSize == 0){
		return 0;
	}
	u32 rval = FINAL_PAGE_START_ADDRESS;
	do{
		rval += (NUM_HIGHSCORES_SAVED * _highScoreCellSize);

	}
	while(*(((u32*)rval)+ 1) != 0xFFFFFFFF);


	return (rval - (NUM_HIGHSCORES_SAVED * _highScoreCellSize));

}


u32 TetrisHighScores_Load(){

	SetHighScoreCellSize();
	u32 finalPage = FINAL_PAGE_START_ADDRESS;
	u32 startAddress = SeekLatestHighScoresStartAddress();
	_nextHighScoreSaveAddress = startAddress + (NUM_HIGHSCORES_SAVED * _highScoreCellSize);
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
	//SetHighScoreCellSize();
	if((_nextHighScoreSaveAddress + (_highScoreCellSize * NUM_HIGHSCORES_SAVED)) <= FINAL_PAGE_END_ADDRESS){
		SaveHighScores(_highScores, _nextHighScoreSaveAddress);
		//LoadHighScores(_highScores, _nextHighScoreSaveAddress);
		_nextHighScoreSaveAddress += (_highScoreCellSize * NUM_HIGHSCORES_SAVED);

	}
	else{
		ErasePagePreservingHighScores();
	}


	//TetrisHighScores_Load();
	return _nextHighScoreSaveAddress;
}

u8 TetrisHighScores_AddHighScore(u32 highScore){

	u8 rval = 255;
	HighScore cachedHighScore;
	for(u32 i=0; i<NUM_HIGHSCORES_SAVED; i++){
		HighScore* thisHighScore = &_highScores[i];

		if(highScore > thisHighScore->Score && rval == 255){
			cachedHighScore = *thisHighScore;
			rval = i;
			thisHighScore->Score = highScore;
			thisHighScore->Initials[0] = 'A';
			thisHighScore->Initials[1] = 'A';
			thisHighScore->Initials[2] = 'A';

		}
		if(i > rval){
			HighScore cachedHighScore2 = *thisHighScore;
			thisHighScore->Score = cachedHighScore.Score;
			thisHighScore->Initials[0] = cachedHighScore.Initials[0];
			thisHighScore->Initials[1] = cachedHighScore.Initials[1];
			thisHighScore->Initials[2] = cachedHighScore.Initials[2];
			cachedHighScore = cachedHighScore2;

		}

	}
	return rval;
}















