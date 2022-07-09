/*
 * TetrisEnterHighScore.h
 *
 *  Created on: Jul 9, 2022
 *      Author: James.Marshall
 */

#ifndef INC_TETRISENTERHIGHSCORE_H_
#define INC_TETRISENTERHIGHSCORE_H_

#include "TetrisHighLevelModesStateMachineDefs.h"

void TetrisEnterHighScore_OnEnter(void* stateMachineDataPtr, Tetris_Modes_States previousState);
void TetrisEnterHighScore_OnExit(void* stateMachineDataPtr, Tetris_Modes_States nextState);
Tetris_Modes_StateTriggers TetrisEnterHighScore_Update(u32 TimePassed);


#endif /* INC_TETRISENTERHIGHSCORE_H_ */
