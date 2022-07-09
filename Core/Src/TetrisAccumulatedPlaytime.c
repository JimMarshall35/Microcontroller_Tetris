/*
 * TetrisAccumulatedPlaytime.c
 *
 *  Created on: Jul 9, 2022
 *      Author: James.Marshall
 */
#include "TetrisAccumulatedPlaytime.h"
#include "main.h"
#include "stm32f3xx_hal_flash_ex.h"

static u32 _accumulatedPlaytime = 0;//0xdeadbeef;

u32 TetrisAccumulatedPlaytime_Load(u32 startAddress){
	_accumulatedPlaytime = *((u32*)startAddress);
	return startAddress += sizeof(u32);
}


u32 TetrisAccumulatedPlaytime_Save(u32 startAddress){
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,startAddress, _accumulatedPlaytime);
	return startAddress += sizeof(u32);
}

u32 TetrisAccumulatedPlaytime_GetAccumulatedPlaytime(){
	return _accumulatedPlaytime;
}

u32 TetrisAccumulatedPlaytime_IncrementAccumulatedPlaytime(u32 amountToIncrementBy){
	_accumulatedPlaytime += amountToIncrementBy;
}
