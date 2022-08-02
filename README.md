# Microcontroller_Tetris

a tetris game made out of:
  - an stm32 nucleo development board (f303k8)
  - a nokia 5110 screen
  - an analog stick
  - a button
  - a passive buzzer

- It plays the tetris music and saves high scores
- It is supposed to play the same as tetris for the gameboy.
- Builds upon a preexisting library for the screen, adding the ability to draw lines, boxes, circles, pixels, ect.

Still to do:
Software:
  - make level select into more of a menu with options to turn the music on and off, ect
  - display high scores on this menu by scrolling them
  - add the option to go back to the main menu from playagain
  - add a title screen
  - change linker script to create a rom which has a default high score in the correct area of flash
  - revisit high score saving, use eeprom emulation library
  
Hardware:
  - figure out a good way to get better sound
  - get a battery and make it work with it and be rechargable
  - make en enclosure for the board
  - add volume control
  - wire up backlight with switch
  - (possibly) make another one with an 8 bit stm chip or a cheaper one without so many unnecessary features and buttons instead of an analog stick
  - (possibly) design a proper pcb
