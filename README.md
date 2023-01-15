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
- draw lines, circles, sqares, bitmaps, see LCDGraphics.c, builds on the lcd_pcd8544_ll.c low level driver, also uses the existing text drawing functionality in lcd_pcd8544.c

https://github.com/evovch/STM32-LCD_PCD8544
