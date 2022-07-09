/*
 * TetrisMain.c
 *
 *  Created on: Jul 5, 2022
 *      Author: James.Marshall
 */

#include "TetrisMain.h"
#include "TetrisGame.h"
#include "TetrisLevelSelectScreen.h"
#include "TetrisPlayAgainScreen.h"
#include "TetrisHighLevelModesStateMachine.h"

#define LEVEL_SELECT_NUM_TRANSITIONS 1
#define PLAYING_NUM_TRANSITIONS 1
#define PLAY_AGAIN_SCREEN_NUM_TRANSITIONS 1

void* _stateMachineDataPointer = 0;
void TetrisMain_SetStateMachineDataPointer(void* dataPtr){
	_stateMachineDataPointer = dataPtr;
}

const Transition _levelSelectTransitions[LEVEL_SELECT_NUM_TRANSITIONS] = {
		{ Playing, StartPlaying }
};

const Transition _playingLevelSelectTransitions[PLAYING_NUM_TRANSITIONS] = {
		{ PlayAgainDialogue, GameOver }
};

const Transition _playAgainScreenTransitions[PLAY_AGAIN_SCREEN_NUM_TRANSITIONS] = {
		{ Playing, StartPlaying }
};

State _states[NumStates];
Tetris_Modes_States _currentState = Playing;

void TetrisMain_Init(){
	TetrisGame_Init();
	u8 startLevel = 8;
	TetrisGame_OnEnter(&startLevel);
	_states[Playing].NumTransitions = PLAYING_NUM_TRANSITIONS;
	_states[Playing].Transitions = _playingLevelSelectTransitions;
	_states[Playing].UpdateFunction = &TetrisGame_Update;
	_states[Playing].OnEnterFunction = &TetrisGame_OnEnter;
	_states[Playing].OnExitFunction = &TetrisGame_OnExit;

	_states[LevelSelect].NumTransitions = LEVEL_SELECT_NUM_TRANSITIONS;
	_states[LevelSelect].Transitions = _levelSelectTransitions;
	_states[LevelSelect].UpdateFunction = &TetrisLevelSelect_Update;
	_states[LevelSelect].OnEnterFunction = &TetrisLevelSelect_OnEnter;
	_states[LevelSelect].OnExitFunction = &TetrisLevelSelect_OnExit;

	_states[PlayAgainDialogue].NumTransitions = PLAY_AGAIN_SCREEN_NUM_TRANSITIONS;
	_states[PlayAgainDialogue].Transitions = _playAgainScreenTransitions;
	_states[PlayAgainDialogue].UpdateFunction = &TetrisPlayAgain_Update;
	_states[PlayAgainDialogue].OnEnterFunction = &TetrisPlayAgain_OnEnter;
	_states[PlayAgainDialogue].OnExitFunction = &TetrisPlayAgain_OnExit;


}

void TetrisMain_Update(u32 timePassed){
	Tetris_Modes_StateTriggers updateResult = _states[_currentState].UpdateFunction(timePassed);
	if(updateResult == NoChange){
		return;
	}
	u32 numTransitions = _states[_currentState].NumTransitions;
	Transition* transitions = _states[_currentState].Transitions;
	for(i32 i=0; i<numTransitions; i++){
		if(transitions[i].trigger == updateResult){
			_states[_currentState].OnExitFunction(_stateMachineDataPointer);
			_currentState = transitions[i].destination;
			_states[_currentState].OnEnterFunction(_stateMachineDataPointer);
		}
	}
}

