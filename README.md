# stm32-microcontroller-nokia-screen
Arsing around with a 5110 nokia screen and a microcontroller (and a ps2 analog stick).

uses this library and adds some extra things to it such as the ability to draw lines, pixels and circles (see LcdGraphics.c).
Draw to a buffer and then it will update only those areas of the lcd screen you have drawn to. THis is not 100% efficient yet though but still better than
re drawing the entire lcd screen.
https://github.com/evovch/STM32-LCD_PCD8544
