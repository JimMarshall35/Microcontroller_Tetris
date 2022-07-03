# Microcontroller_Tetris

Tetris for a 32 bit arm microcontroller - I've used a NUCLEO-F303K8
but with some tweaking it would probably work on many different stm microcontrollers.

Uses these components:
- Nokia 5110 screen 
- NUCLEO-F303K8 development board
- PS2 analog stick
- Push button

uses this library for the screen and expands on it (see LcdGraphics.c / .h)
https://github.com/evovch/STM32-LCD_PCD8544

I'd like to add the ability to save high scores and get it to work with a battery.

The software isn't fully complete yet either there's a few things i still need to do - firstly seed the random number generator somehow.
