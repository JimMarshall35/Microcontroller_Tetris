/*
 * TetrisHighLevelModesStateMachineDefs.h
 *
 *  Created on: Jul 5, 2022
 *      Author: James.Marshall
 */

#ifndef INC_TETRISHIGHLEVELMODESSTATEMACHINEDEFS_H_
#define INC_TETRISHIGHLEVELMODESSTATEMACHINEDEFS_H_
#include "BasicTypedefs.h"


typedef enum{
	NoChange,
	StartPlaying,
	GameOver,
	GameOverNewHighScore,
	GoToLevelSelectScreen,
	HighScoreEntered,
	LookAtHighScores,
	NumTriggers
}Tetris_Modes_StateTriggers;

typedef enum{
	LevelSelect,
	Playing,
	PlayAgainDialogue,
	EnterNewHighScoreScreen,
	ViewHighScoresScreen,
	NoState,

	NumStates,
}Tetris_Modes_States;

typedef Tetris_Modes_StateTriggers (*UpdateStateFunction)(u32 timePassed);
typedef void (*EnterStateFunction)(void* data, Tetris_Modes_States previousState);
typedef void (*ExitStateFunction)(void* data, Tetris_Modes_States nextState);

typedef struct{
	Tetris_Modes_States destination;
	Tetris_Modes_StateTriggers trigger;
}Transition;

typedef struct{
	UpdateStateFunction UpdateFunction;
	EnterStateFunction OnEnterFunction;
	ExitStateFunction OnExitFunction;
	u8 NumTransitions;
	const Transition* Transitions;
}State;


#endif /* INC_TETRISHIGHLEVELMODESSTATEMACHINEDEFS_H_ */
