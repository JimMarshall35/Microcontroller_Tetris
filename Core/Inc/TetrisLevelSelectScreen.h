/*
 * TetrisLevelSelectScreen.h
 *
 *  Created on: Jul 6, 2022
 *      Author: James.Marshall
 */

#ifndef INC_TETRISLEVELSELECTSCREEN_H_
#define INC_TETRISLEVELSELECTSCREEN_H_

#include <TetrisHighLevelModesStateMachineDefs.h>
#include "BasicTypedefs.h"

Tetris_Modes_StateTriggers TetrisLevelSelect_Update(u32 TimePassed);
void TetrisLevelSelect_Init();
void TetrisLevelSelect_OnEnter(void* stateMachineDataPtr, Tetris_Modes_States previousState);
void TetrisLevelSelect_OnExit(void* stateMachineDataPtr, Tetris_Modes_States nextState);


#endif /* INC_TETRISLEVELSELECTSCREEN_H_ */
