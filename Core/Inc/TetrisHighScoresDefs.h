/*
 * TetrisHighScoresDefs.h
 *
 *  Created on: Jul 9, 2022
 *      Author: James.Marshall
 */

#ifndef INC_TETRISHIGHSCORESDEFS_H_
#define INC_TETRISHIGHSCORESDEFS_H_

#include "BasicTypedefs.h"


#define NUM_HIGHSCORES_SAVED 5

typedef struct{
	char Initials[3];
	u32 Score;
}HighScore;

#define SIZE_OF_KB_IN_BYTES 1024
/* defines for stmfk303k8 */
#define NUM_KB_PER_FLASH_PAGE 2
#define NUM_BYTES_PER_FLASH_PAGE (SIZE_OF_KB_IN_BYTES * NUM_KB_PER_FLASH_PAGE)
#define BASE_FLASH_ADDRESS 0x08000000
#define NUM_FLASH_PAGES 32
#define FINAL_PAGE_START_ADDRESS (BASE_FLASH_ADDRESS + ((NUM_FLASH_PAGES - 1) * NUM_BYTES_PER_FLASH_PAGE)) // 0x800f800

#endif /* INC_TETRISHIGHSCORESDEFS_H_ */
