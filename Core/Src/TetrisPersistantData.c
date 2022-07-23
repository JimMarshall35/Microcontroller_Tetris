/*
 * TetrisPersistantData.c
 *
 *  Created on: Jul 9, 2022
 *      Author: James.Marshall
 *
 * To save data to flash rom on an f303k8 microcontroller
 * you have to first erase it, which you can only do in "pages" of 2kb.
 *
 * In the tetris game the final 2kb page is reserved for persistent game
 * data like high scores and the accumulated play time counter which is used for
 * RNG and so there is 2kb less for code than there would otherwise be.
 *
 * When saved the entirety of the persistent data must be saved together
 * and so this file provides a function that creates an unlocked flash context,
 * erases the page and then writes all the persistent data together calling all functions
 * from other modules that save flash data while the flash is unlocked and last page is freshly
 * erased.
 */


#include "TetrisPersistantData.h"
#include "TetrisHighScores.h"
#include "main.h"
#include "stm32f3xx_hal_flash_ex.h"

void TetrisPersistantData_LoadAllPeristantData(){
	u32 nextAddress = TetrisHighScores_Load();

}
void TetrisPersistantData_SaveAllPersistantData(){
	HAL_FLASH_Unlock();

	/*
	FLASH_EraseInitTypeDef eraseInit;
	eraseInit.NbPages = 1;
	eraseInit.PageAddress = FINAL_PAGE_START_ADDRESS;
	eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;

	u32 pageError;
	HAL_FLASHEx_Erase(&eraseInit,&pageError);
	*/

	u32 nextAddress = TetrisHighScores_Save(FINAL_PAGE_START_ADDRESS);

	HAL_FLASH_Lock();

}
