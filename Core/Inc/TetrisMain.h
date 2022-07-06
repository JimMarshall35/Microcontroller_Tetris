/*
 * TetrisMain.h
 *
 *  Created on: Jul 5, 2022
 *      Author: James.Marshall
 *
 * The main tetris game loop that brings
 * together the other screens of the game
 */

#ifndef SRC_TETRISMAIN_H_
#define SRC_TETRISMAIN_H_

#include "BasicTypedefs.h"

void TetrisMain_Init();
void TetrisMain_Update(u32 timePassed);
void TetrisMain_SetStateMachineDataPointer(void* dataPtr);

#endif /* SRC_TETRISMAIN_H_ */
