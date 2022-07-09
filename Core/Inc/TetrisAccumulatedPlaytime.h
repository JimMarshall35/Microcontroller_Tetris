/*
 * TetrisAccumulatedPlaytime.h
 *
 *  Created on: Jul 9, 2022
 *      Author: James.Marshall
 */

#ifndef SRC_TETRISACCUMULATEDPLAYTIME_H_
#define SRC_TETRISACCUMULATEDPLAYTIME_H_
#include "BasicTypedefs.h"

/*
 * takes start address and returns next avaliable address
 * in rom for reading from. See TetrisPersistanceData.c
 * for usage
 * */
u32 TetrisAccumulatedPlaytime_Load(u32 startAddress);
u32 TetrisAccumulatedPlaytime_Save(u32 startAddress);

u32 TetrisAccumulatedPlaytime_GetAccumulatedPlaytime();
u32 TetrisAccumulatedPlaytime_IncrementAccumulatedPlaytime(u32 amountToIncrementBy);

#endif /* SRC_TETRISACCUMULATEDPLAYTIME_H_ */
