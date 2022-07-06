/*
 * TetrisHighLevelModesStateMachine.h
 *
 *  Created on: Jul 5, 2022
 *      Author: James.Marshall
 */

#ifndef INC_TETRISHIGHLEVELMODESSTATEMACHINE_H_
#define INC_TETRISHIGHLEVELMODESSTATEMACHINE_H_
#include "BasicTypedefs.h"


typedef enum{
	NoChange,
	StartPlaying,
	GameOver,
	GoToLevelSelectScreen,
	NumTriggers
}Tetris_Modes_StateTriggers;

typedef enum{
	LevelSelect,
	Playing,
	PlayAgainDialogue,
	NumStates
}Tetris_Modes_States;

typedef Tetris_Modes_StateTriggers (*UpdateStateFunction)(u32 timePassed);
typedef void (*EnterStateFunction)(void* data);

typedef struct{
	Tetris_Modes_States destination;
	Tetris_Modes_StateTriggers trigger;
}Transition;

typedef struct{
	UpdateStateFunction UpdateFunction;
	EnterStateFunction OnEnterFunction;
	u8 NumTransitions;
	const Transition* Transitions;
}State;


#endif /* INC_TETRISHIGHLEVELMODESSTATEMACHINE_H_ */
