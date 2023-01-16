# Microcontroller_Tetris

tetris for an stm32 microcontroller - uses the stm HAL.

parts needed:
- nokia 5110 screen
- button
- analog stick
- piezoelectric buzzer (passive)
- stm32 f303k8 nucleo development board (would probably work with others)

features:
- levels, which match gameboy tetris for speed and score
- saves high scores to flash memory
- plays the tetris music

adds graphical capabilities to a nokia 5110 screen driver by github user evovch
- draw lines, circles, squares, bitmaps, see LCDGraphics.c, builds on the lcd_pcd8544_ll.c low level driver, also uses the existing text drawing functionality in lcd_pcd8544.c

https://github.com/evovch/STM32-LCD_PCD8544


Plays the tetris music by using a timer interrupt to automatically toggle a pin generating a square wave with no involvement from the CPU, and another timer interrupt with an ISR and a longer period between interrupts to sequence the notes that make up the tune

Saves high score to flash with wear leveling, filling up the final 2kb page with high scores and only erasing the page and carrying the high scores over when the page is full (it does this sub-optimally, saving the entire block of six high scores each time when it could only save the highest one, although this way does make it faster to seek the latest high scores on startup)
