/*
 * LcdGraphics.h
 *
 *  Created on: 12 Jun 2022
 *      Author: James.Marshall
 */

#ifndef INC_LCDGRAPHICS_H_
#define INC_LCDGRAPHICS_H_

#define PIXELS_WIDTH 84
#define PIXELS_HEIGHT 48
#define SCREEN_NUM_ROWS 6

#include <stdint.h>
#include "BasicTypedefs.h"


// typedefs
typedef struct{
	u8 updateNeeded;
	u8 updateColumnStart;
	u8 updateColumnFinish;
}ScreenRow;

// forward declarations
struct LCD_PCD8544_screen_t;
extern unsigned char gFrameBuffer[];
extern ScreenRow gScreenRegionsToUpdate[SCREEN_NUM_ROWS];

// functions
void gfxPlotPixel(u8 x, u8 y);
void gfxClearFrameBuffer();
void gfxCopyAreaOfFrameBufferToScreen(LCD_PCD8544_screen_t* scr, u8 rowStart, u8 rowStop, u8 colStart, u8 colStop); // rows not per pixel, per byte row 0-5
void gfxDrawLine(u8 x1, u8 y1, u8 x2, u8 y2);
void gfxFinishDrawing(LCD_PCD8544_screen_t* scr);
void gfxWriteTextLineToFrameBuffer(u8 vIndex, u8 hIndex, const char* string);
void gfxMidPointCircleDraw(int x_centre, int y_centre, int r);
#endif /* INC_LCDGRAPHICS_H_ */
