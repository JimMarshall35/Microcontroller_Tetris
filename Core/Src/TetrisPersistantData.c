/*
 * TetrisPersistantData.c
 *
 *  Created on: Jul 9, 2022
 *      Author: James.Marshall
 */


#include "TetrisPersistantData.h"
#include "TetrisHighScores.h"
#include "TetrisAccumulatedPlaytime.h"
#include "main.h"
#include "stm32f3xx_hal_flash_ex.h"

void TetrisPersistantData_LoadAllPeristantData(){
	u32 nextAddress = TetrisHighScores_Load(FINAL_PAGE_START_ADDRESS);
	nextAddress = TetrisAccumulatedPlaytime_Load(nextAddress);

}
void TetrisPersistantData_SaveAllPersistantData(){
	HAL_FLASH_Unlock();

	FLASH_EraseInitTypeDef eraseInit;
	eraseInit.NbPages = 1;
	eraseInit.PageAddress = FINAL_PAGE_START_ADDRESS;
	eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;

	u32 pageError;
	HAL_FLASHEx_Erase(&eraseInit,&pageError);

	u32 nextAddress = TetrisHighScores_Save(FINAL_PAGE_START_ADDRESS);
	nextAddress = TetrisAccumulatedPlaytime_Save(nextAddress);

	HAL_FLASH_Lock();

}
