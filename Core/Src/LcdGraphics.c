/*
 * LcdGraphics.c
 *
 *  Created on: 12 Jun 2022
 *      Author: James.Marshall
 */
#include "lcd_pcd8544_ll.h"
#include "font6x8.h"

#include "LcdGraphics.h"
#define Swapi8(v1, v2) i8 temp = v1; v1 = v2; v2 = temp;
#define Swapf32(v1, v2) f32 temp = v1; v1 = v2; v2 = temp;

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))



u8 gFrameBuffer[(PIXELS_HEIGHT/8)*PIXELS_WIDTH];

ScreenRow gScreenRegionsToUpdate[SCREEN_NUM_ROWS] = {
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
};


// clear the frame buffer and set all regions to needing updating
void gfxClearFrameBuffer();

//plot a single pixel, updating the screen regions to update data structure
void gfxPlotPixel(u8 x, u8 y);
//plot a single pixel, not updating the screen regions to update data structure.
// used internally
void gfxPlotPixelInternal(u8 x, u8 y);

// copy a rectangular area of the frame buffer to the screen
void gfxCopyAreaOfFrameBufferToScreen(LCD_PCD8544_screen_t* scr, u8 rowStart, u8 rowStop, u8 colStart, u8 colStop);

// the publicly exposed line drawing function and its two sub functions
void gfxDrawLine(u8 x0, u8 y0, u8 x1, u8 y1);
// https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
void gfxDrawLineBresenhamLow(u8 x0, u8 y0, u8 x1, u8 y1);
void gfxDrawLineBresenhamHigh(u8 x0, u8 y0, u8 x1, u8 y1);
void gfxWriteTextLineToFrameBuffer(u8 vIndex, u8 hIndex, const char* string);


// helpers to update the global variable gScreenRegionsToUpdate
void UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(u8 rowStart, u8 rowStop, u8 colStart, u8 colStop);
void UpdateScreenRegionsToUpdate_SinglePixel(u8 x, u8 y);
void UpdateScreenRegionsToUpdate_ClearedFrameBuffer();
void UpdateScreenRegionsToUpdate_LineDrawn(u8 x0, u8 y0, u8 x1, u8 y1);
void UpdateScreenRegionsToUpdate_TextDrawn(u8 vIndex, u8 hIndex, u8 stringWidthBytes);

// lower level functions
HAL_StatusTypeDef gfxWriteBytes(LCD_PCD8544_screen_t* scr,
                                          unsigned char vIndex, unsigned char hIndex,
                                          unsigned char* bytes, unsigned short int size);

// copy all the regions of the frame buffer that are marked as needing to be updated
// to the lcd screen
void gfxFinishDrawing(LCD_PCD8544_screen_t* scr);


void UpdateScreenRegionsToUpdate_ClearedFrameBuffer(){
	for(int i=0; i < SCREEN_NUM_ROWS; i++){
		gScreenRegionsToUpdate[i].updateNeeded = true;
		gScreenRegionsToUpdate[i].updateColumnStart = 0;
		gScreenRegionsToUpdate[i].updateColumnFinish = PIXELS_WIDTH;
	}
}

void gfxClearFrameBuffer(){
	memset(gFrameBuffer, 0, sizeof(gFrameBuffer));
	//UpdateScreenRegionsToUpdate_ClearedFrameBuffer();
}

void gfxClearFrameBufferRect(u8 pixelsTopLeftX, u8 pixelsTopLeftY, u8 pixelsBottomRightX, u8 pixelsBottomRightY){
	/*  */

	int screenYIndexTL = ((pixelsTopLeftY / 8)*PIXELS_WIDTH);


	int screenYIndexBR = ((pixelsBottomRightY / 8)*PIXELS_WIDTH);

	int screenYIndexRemainderTL = pixelsTopLeftY % 8;
	int screenYIndexRemainderBR = pixelsBottomRightY % 8;

	if(screenYIndexBR == screenYIndexTL){
		u32 bitMask = 0;
		for(int pixelRow = screenYIndexRemainderTL; pixelRow <=screenYIndexRemainderBR; pixelRow++){
			bitMask |= (1 << pixelRow);
		}
		for(int i = pixelsTopLeftX; i <= pixelsBottomRightX; i++){
			gFrameBuffer[screenYIndexTL + i] &= ~(bitMask);
		}
	}
	else{
		for(int row = screenYIndexTL; row <= screenYIndexBR; row++){
			if(row == screenYIndexTL){
				u32 bitMask = 0;
				for(int pixelRow = screenYIndexRemainderTL; row < 8; pixelRow++){
					bitMask |= (1 << pixelRow);
				}
				for(int i = pixelsTopLeftX; i <= pixelsBottomRightX; i++){
					gFrameBuffer[screenYIndexTL + i] &= ~(bitMask);
				}
			}
			else if(row == screenYIndexBR){
				u32 bitMask = 0;
				for(int pixelRow = screenYIndexRemainderTL; row >= 0; pixelRow--){
					bitMask |= (1 << pixelRow);

				}
				for(int i = pixelsTopLeftX; i <= pixelsBottomRightX; i++){
					gFrameBuffer[screenYIndexBR + i] &= ~(bitMask);
				}

			}
			else{
				memset(gFrameBuffer + (row * PIXELS_WIDTH)+pixelsTopLeftX, 0, (pixelsBottomRightX - pixelsTopLeftX));
			}
		}
	}


}

void gfxPlotPixelInternal(u8 x, u8 y){
	if(x >= 84 || y >= 47){
		return;
	}
	int indexOfSliceContainingPixel = ((y / 8)*PIXELS_WIDTH) + x;
	if(indexOfSliceContainingPixel < sizeof(gFrameBuffer))
		gFrameBuffer[indexOfSliceContainingPixel] |= (1 << (y % 8));
}

void UpdateScreenRegionsToUpdate_SinglePixel(u8 x, u8 y){
	u8 row = y / SCREEN_NUM_ROWS;
	if(gScreenRegionsToUpdate[row].updateNeeded == false){
		gScreenRegionsToUpdate[row].updateNeeded = true;
		gScreenRegionsToUpdate[row].updateColumnStart = x;
		gScreenRegionsToUpdate[row].updateColumnFinish = x;
	}
	else{
		if(x > gScreenRegionsToUpdate[row].updateColumnFinish){
			gScreenRegionsToUpdate[row].updateColumnFinish = x;
		}
		else if(x < gScreenRegionsToUpdate[row].updateColumnStart){
			gScreenRegionsToUpdate[row].updateColumnStart = x;
		}

	}
}

void gfxPlotPixel(u8 x, u8 y){
	gfxPlotPixelInternal(x,y);
	UpdateScreenRegionsToUpdate_SinglePixel(x,y);
}

void UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(u8 rowStart, u8 rowStop, u8 colStart, u8 colStop){
	for(int row = rowStart; row <= rowStop; row++){
		gScreenRegionsToUpdate[row].updateNeeded = true;
		gScreenRegionsToUpdate[row].updateColumnStart = colStart;
		gScreenRegionsToUpdate[row].updateColumnFinish = colStop;
	}
}

void gfxCopyAreaOfFrameBufferToScreen(LCD_PCD8544_screen_t* scr, u8 rowStart, u8 rowStop, u8 colStart, u8 colStop){
	for(int i=rowStart; i<=rowStop; i++){
		gScreenRegionsToUpdate[i].updateNeeded = false;
		int yOffset = i * PIXELS_WIDTH;
		gfxWriteBytes(scr,i,colStart, &gFrameBuffer[yOffset + colStart], colStop-colStart);
	}
	UpdateScreenRegionsToUpdate_FrameBufferRectCopiedToScreen(rowStart,rowStop,colStart,colStop);
}

void gfxDrawLineNaive(u8 x1, u8 y1, u8 x2, u8 y2){
	i8 rise = y2 - y1;
	i8 run = x2 - x1;
	f32 m = (f32)rise / (f32)run;
	f32 c = (f32)y1 - m*(f32)x1;
	if(run == 0){
		if(y2 < y1){
			Swapi8(y1,y2);
		}
		for(int y=y1; y<=y2; y++){
			gfxPlotPixelInternal(x1,y);
		}
	}
	else{
		f32 m = (f32)rise / (f32)run;
		f32 c = (f32)y1 - (f32)m*x1;
		if(m <= 1.0f && m >= -1.0f){
			if(x2 < x1){
				Swapi8(x1,x2);
			}
			for(u8 x = x1; x <= x2; x++){
				u8 y = (u8)round(m * (f32)x + c);
				gfxPlotPixelInternal(x,y);
			}
		}
		else{
			if(y2 < y1){
				Swapi8(y1,y2);
			}
			for(int y=y1; y<=y2; y++){
				u8 x = (u8)round((y-c) / m);
				gfxPlotPixelInternal(x,y);
			}
		}
	}
	//UpdateScreenRegionsToUpdate_LineDrawn(x1,y1,x2,y2);

}



void gfxDrawLineBresenhamLow(u8 x0, u8 y0, u8 x1, u8 y1){
	i32 dx = x1 - x0;
	i32 dy = y1 - y0;
	i32 yi = 1;
	if(dy < 0){
		yi = -1;
		dy = -dy;
	}
	i32 D = (2 * dy) - dx;
	i32 y = y0;
	for(i32 x = x0; x <= x1; x++){
		gfxPlotPixelInternal(x,y);
		if(D > 0){
			y = y + yi;
			D = D + (2*(dy-dx));
		}
		else{
			D = D + 2*dy;
		}
	}

}

void gfxDrawLineBresenhamHigh(u8 x0, u8 y0, u8 x1, u8 y1){
	i32 dx = x1 - x0;
	i32 dy = y1 - y0;
	i32 xi = 1;
	if (dx < 0){
		xi = 1;
		dx = -dx;
	}
	i32 D = (2 * dx) - dy;
	i32 x = x0;
	for(i32 y = y0; y <= y1; y++){
		gfxPlotPixelInternal(x,y);
		if(D > 0){
			x = x + xi;
			D = D + (2 * (dx - dy));
		}
		else{
			D = D + 2*dx;
		}
	}
}



void gfxDrawLine(u8 x0, u8 y0, u8 x1, u8 y1){
	if(abs(y1 - y0) < abs(x1 - x0)){
		if(x0 > x1){
			gfxDrawLineBresenhamLow(x1,y1,x0,y0);
		}
		else{
			gfxDrawLineBresenhamLow(x0,y0,x1,y1);
		}
	}
	else{
		if(y0 > y1){
			gfxDrawLineBresenhamHigh(x1,y1,x0,y0);
		}
		else{
			gfxDrawLineBresenhamHigh(x0,y0,x1,y1);
		}

	}
	//UpdateScreenRegionsToUpdate_LineDrawn(x0,y0,x1,y1);
}

void UpdateScreenRegionsToUpdate_LineDrawn(u8 x0, u8 y0, u8 x1, u8 y1){
	if(x1 < x0){
		Swapi8(x0,x1);
	}
	if(y1 < y0){
		Swapi8(y0,y1);
	}
	u8 rowStart = y0 / SCREEN_NUM_ROWS;
	u8 rowFinish = y1 / SCREEN_NUM_ROWS;
	for(int row = rowStart; row <= rowFinish; row++){
		gScreenRegionsToUpdate[row].updateNeeded = true;
		gScreenRegionsToUpdate[row].updateColumnStart = x0;
		gScreenRegionsToUpdate[row].updateColumnFinish = x1;
	}

}


void gfxFinishDrawing(LCD_PCD8544_screen_t* scr){
	char info[25];
	for(int row=0; row<SCREEN_NUM_ROWS; row++ ){
		if(gScreenRegionsToUpdate[row].updateNeeded == true){



			u8 colStart = gScreenRegionsToUpdate[row].updateColumnStart;
			u8 colFinish = gScreenRegionsToUpdate[row].updateColumnFinish;
			if(colFinish < colStart){
				Swapi8(colStart,colFinish);
			}
			//sprintf(info,"s: %d f: %d", colStart, colFinish);
			//LCD_PCD8544_write_line(scr,row,info);
			int yOffset = row * PIXELS_WIDTH;

			gfxWriteBytes(scr,row,colStart, &gFrameBuffer[yOffset + colStart], colFinish-colStart);


			gScreenRegionsToUpdate[row].updateNeeded = false;
			gScreenRegionsToUpdate[row].updateColumnStart = 0;
			gScreenRegionsToUpdate[row].updateColumnFinish = 0;
		}
		/*
		else{
			u8 colStart = gScreenRegionsToUpdate[row].updateColumnStart;
			u8 colFinish = gScreenRegionsToUpdate[row].updateColumnFinish;
			sprintf(info,"s: %d f: %d", colStart, colFinish);
			LCD_PCD8544_write_line(scr,row,info);
		}
		*/
	}
}

HAL_StatusTypeDef gfxWriteBytes(LCD_PCD8544_screen_t* scr,
                                          unsigned char vIndex, unsigned char hIndex,
                                          unsigned char* bytes, unsigned short int size){
	// Jims Comment - this is the same as the one in lcd_pcd8544.h buy it
	// takes hIndex as bytes not characters


	//TODO check that vIndex, hIndex, and size are in the allowed range

	// 0 - horizontal addressing;
	// 1 - vertical addressing.
	unsigned char addressing_type = 0;

	// 0 - chip is active;
	// 1 - chip is in power-down mode.
	unsigned char power_down = 0;

	// Set the cursor to (vIndex,hIndex)
	LCD_PCD8544_LL_switch_to_commands_mode(scr);
	LCD_PCD8544_LL_begin_data_transfer(scr);
	// 1st byte: instruction = 'Function set' with H=0
	// With this command sent to the LCD controller we
	// indicate that so called basic instruction set will be used
	LCD_PCD8544_LL_set_function_set(scr, power_down, addressing_type, 0);
	// 2nd byte: instruction = 'Set Y address of RAM'
	// 3rd byte: instruction = 'Set X address of RAM'
	LCD_PCD8544_LL_set_YX_address_of_RAM(scr, vIndex, hIndex);
	LCD_PCD8544_LL_end_data_transfer(scr);

	//TODO can we do both parts within the same batch?

	// Per-se fill the RAM with the provided string
	LCD_PCD8544_LL_switch_to_data_mode(scr);
	LCD_PCD8544_LL_begin_data_transfer(scr);
	HAL_StatusTypeDef stat = LCD_PCD8544_LL_send_data(scr, bytes, size);
	LCD_PCD8544_LL_end_data_transfer(scr);

	return stat;
}

void gfxWriteTextLineToFrameBuffer(u8 vIndex, u8 hIndex, const char* string){
	unsigned short int strLen = strlen(string);
	if (strLen* LCD_PCD8544_CHAR_WIDTH + hIndex > LCD_PCD8544_LINEWIDTH * LCD_PCD8544_CHAR_WIDTH) {
		strLen = LCD_PCD8544_LINEWIDTH;
		hIndex = 0;
	} // cut to the right edge of the screen

	//TODO check that initialization works; maybe better use memset?
	unsigned char data[LCD_PCD8544_LINEWIDTH*LCD_PCD8544_CHAR_WIDTH];
	memset(data, 0, sizeof(data));

	// Translate the input string into the bit array using the font
	unsigned short int nChars=0;
	for ( ; nChars<strLen; nChars++) {
		if (string[nChars] == '\0') {
			//TODO check
			break;
		} else {
			unsigned short int idx = (unsigned short int)(string[nChars]);
			memcpy(&data[LCD_PCD8544_CHAR_WIDTH*nChars], &gFont6x8[LCD_PCD8544_CHAR_WIDTH*idx], LCD_PCD8544_CHAR_WIDTH);
		}
	}
	int yOffset = vIndex * PIXELS_WIDTH;
	u8 stringWidthBytes = strLen*LCD_PCD8544_CHAR_WIDTH;
	memcpy(&gFrameBuffer[yOffset + hIndex], data, stringWidthBytes);
	//UpdateScreenRegionsToUpdate_TextDrawn(vIndex, hIndex, stringWidthBytes);

}


void UpdateScreenRegionsToUpdate_TextDrawn(u8 vIndex, u8 hIndex, u8 stringWidthBytes){
	gScreenRegionsToUpdate[vIndex].updateNeeded = true;
	u8 currentUpdateColStart = gScreenRegionsToUpdate[vIndex].updateColumnStart;
	u8 currentUpdateColFinish = gScreenRegionsToUpdate[vIndex].updateColumnFinish;
	gScreenRegionsToUpdate[vIndex].updateColumnStart = MIN(currentUpdateColStart, hIndex);
	gScreenRegionsToUpdate[vIndex].updateColumnFinish = MAX(currentUpdateColFinish, hIndex + stringWidthBytes);
}


// Implementing Mid-Point Circle Drawing Algorithm
void gfxMidPointCircleDraw(int x_centre, int y_centre, int r)
{
    int x = r, y = 0;

    // Printing the initial point on the axes
    // after translation
    gfxPlotPixel(x - x_centre, y - y_centre);
    gfxPlotPixel(x + x_centre, y + y_centre);

    // When radius is zero only a single
    // point will be printed
    if (r > 0)
    {
    	gfxPlotPixel(x + x_centre, -y + y_centre);
    	gfxPlotPixel(y + x_centre, x + y_centre);
    	gfxPlotPixel(-y + x_centre, x + y_centre);
    }

    // Initialising the value of P
    int P = 1 - r;
    while (x > y)
    {
        y++;

        // Mid-point is inside or on the perimeter
        if (P <= 0)
            P = P + 2*y + 1;

        // Mid-point is outside the perimeter
        else
        {
            x--;
            P = P + 2*y - 2*x + 1;
        }

        // All the perimeter points have already been printed
        if (x < y)
            break;

        // Printing the generated point and its reflection
        // in the other octants after translation
        gfxPlotPixel(x + x_centre, y + y_centre);
        gfxPlotPixel(-x + x_centre, y + y_centre);
        gfxPlotPixel(x + x_centre, -y + y_centre);
        gfxPlotPixel(-x + x_centre, -y + y_centre);

        // If the generated point is on the line x = y then
        // the perimeter points have already been printed
        if (x != y)
        {
        	gfxPlotPixel(y + x_centre, x + y_centre);
        	gfxPlotPixel(-y + x_centre, x + y_centre);
        	gfxPlotPixel(y + x_centre, -x + y_centre);
            gfxPlotPixel(-y + x_centre, -x + y_centre);
        }
    }
}


void gfxDrawAxisAlignedRect(u8 tlX, u8 tlY, u8 brX, u8 brY){
	gfxDrawLine(tlX,tlY, brX,tlY);
	gfxDrawLine(brX,tlY, brX, brY);
	gfxDrawLine(brX, brY, tlX, brY);
	gfxDrawLine(tlX, brY, tlX,tlY);


}

void ClearUpdateRegions(){
	for(int i=0; i < SCREEN_NUM_ROWS; i++){
		gScreenRegionsToUpdate[i].updateColumnFinish = 0;
		gScreenRegionsToUpdate[i].updateColumnStart = 0;
		gScreenRegionsToUpdate[i].updateNeeded = 0;


	}
}


void ClearScreen(LCD_PCD8544_screen_t* scr){
	gfxClearFrameBuffer();
	for(int i=0; i < SCREEN_NUM_ROWS; i++){
		gScreenRegionsToUpdate[i].updateColumnFinish = 83;
		gScreenRegionsToUpdate[i].updateColumnStart = 0;
		gScreenRegionsToUpdate[i].updateNeeded = 1;
	}
	gfxFinishDrawing(scr);

}


void gfxClearFrameBufferRow(u8 rowIndex, u8 colStart, u8 amount){
	u8* frameBufferStartPtr = &gFrameBuffer[(rowIndex * PIXELS_WIDTH) + colStart];

	memset(frameBufferStartPtr, 0, amount);

}















