/*
 * TetrisHighScorePrintHelpers.c
 *
 *  Created on: 3 Sep 2022
 *      Author: James.Marshall
 */

#include "TetrisHighScorePrintHelpers.h"
#include "LcdGraphics.h"



void Tetris_WriteSingleHighScoreToFrameBuffer(const HighScore* highScore, u8 line){
	char highScoreLineBuffer[10];
	sprintf(highScoreLineBuffer,"%c%c%c %lu",
		highScore->Initials[0],
		highScore->Initials[1],
		highScore->Initials[2],
		highScore->Score);
	gfxWriteTextLineToFrameBuffer(line,0,highScoreLineBuffer);

}

void Tetris_DrawHighScoresToFrameBuffer(){
	for(u32 i=0; i < NUM_HIGHSCORES_SAVED; i++){
		const HighScore* highScore = TetrisHighScores_GetHighScoreAtIndex(i);
		Tetris_WriteSingleHighScoreToFrameBuffer(highScore, i+1);
	}

}
