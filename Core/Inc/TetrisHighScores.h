/*
 * TetrisHighScores.h
 *
 *  Created on: Jul 9, 2022
 *      Author: James.Marshall
 */

#ifndef INC_TETRISHIGHSCORES_H_
#define INC_TETRISHIGHSCORES_H_

#include "TetrisHighScoresDefs.h"

/*
 * takes start address and returns next avaliable address
 * in rom for reading from. See TetrisPersistanceData.c
 * for usage
 * */
u32 TetrisHighScores_Load(u32 startAddress);
/*
 * takes start address and returns next avaliable address
 * in rom for reading from. See TetrisPersistanceData.c
 * for usage
 * */
u32 TetrisHighScores_Save(u32 startAddress);

/*
 * returns true if the submitted score is a new high score and
 * false if not
 * */
bool TetrisHighScores_IsNewHighScore(u32 potentialHighScore);

HighScore* TetrisHighScores_GetHighScoreAtIndex(u8 atIndex);

/*
 * to be called passing a known high score, one that's confirmed by
 * TetrisHighScores_IsNewHighScore.
 *
 * Inserts it into the high scores data structure and returns the new ranking
 * with 0 being first place and 4 being last
 * */
u8 TetrisHighScores_AddHighScore(u32 highScore);


//void LoadHighScores(HighScore* output);
//void SaveHighScores(HighScore* input);

#endif /* INC_TETRISHIGHSCORES_H_ */
