/*
 * TetrisHighScorePrintHelpers.h
 *
 *  Created on: 3 Sep 2022
 *      Author: James.Marshall
 */

#ifndef INC_TETRISHIGHSCOREPRINTHELPERS_H_
#define INC_TETRISHIGHSCOREPRINTHELPERS_H_

#include "BasicTypedefs.h"
#include "TetrisHighScoresDefs.h"


struct HighScore;

void Tetris_DrawHighScoresToFrameBuffer();
void Tetris_WriteSingleHighScoreToFrameBuffer(const HighScore* highScore, u8 line);

#endif /* INC_TETRISHIGHSCOREPRINTHELPERS_H_ */
