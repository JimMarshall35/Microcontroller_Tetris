/*
 * TetrisPlayAgainScreen.h
 *
 *  Created on: Jul 6, 2022
 *      Author: James.Marshall
 */

#ifndef INC_TETRISPLAYAGAINSCREEN_H_
#define INC_TETRISPLAYAGAINSCREEN_H_

Tetris_Modes_StateTriggers TetrisPlayAgain_Update(u32 TimePassed);
void TetrisPlayAgain_Init();
void TetrisPlayAgain_OnEnter(void* stateMachineDataPtr, Tetris_Modes_States previousState);
void TetrisPlayAgain_OnExit(void* stateMachineDataPtr, Tetris_Modes_States nextState);

#endif /* INC_TETRISPLAYAGAINSCREEN_H_ */
