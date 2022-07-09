/*
 * TetrisGame.H
 *
 *  Created on: Jun 29, 2022
 *      Author: James.Marshall
 */

#ifndef INC_TETRISGAME_H_
#define INC_TETRISGAME_H_
#include "TetrisHighLevelModesStateMachineDefs.h"
#include "BasicTypedefs.h"

Tetris_Modes_StateTriggers TetrisGame_Update(u32 TimePassed);
void TetrisGame_Init();
void TetrisGame_OnEnter(void* stateMachineDataPtr, Tetris_Modes_States previousState);
void TetrisGame_OnExit(void* stateMachineDataPtr, Tetris_Modes_States nextState);

#endif /* INC_TETRISGAME_H_ */
