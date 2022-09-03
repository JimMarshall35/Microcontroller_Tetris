/*
 * TetrisViewHighScores.h
 *
 *  Created on: 3 Sep 2022
 *      Author: James.Marshall
 */

#ifndef INC_TETRISVIEWHIGHSCORES_H_
#define INC_TETRISVIEWHIGHSCORES_H_

#include "TetrisHighLevelModesStateMachineDefs.h"

Tetris_Modes_StateTriggers TetrisViewHighScores_Update(u32 TimePassed);
void TetrisViewHighScores_OnEnter(void* stateMachineDataPtr, Tetris_Modes_States previousState);
void TetrisViewHighScores_OnExit(void* stateMachineDataPtr, Tetris_Modes_States nextState);


#endif /* INC_TETRISVIEWHIGHSCORES_H_ */
