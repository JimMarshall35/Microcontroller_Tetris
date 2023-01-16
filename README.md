# Microcontroller_Tetris

tetris for an stm32 microcontroller - uses the stm HAL.

parts needed:
- nokia 5110 screen
- button
- analog stick (it would be better with a d-pad but I only had an analog stick and only one button)
- piezoelectric buzzer (passive)
- stm32 f303k8 nucleo development board (would probably work with others)

features:
- levels, which match gameboy tetris for speed and score
- saves high scores to flash memory
- plays the tetris music

adds graphical capabilities to a nokia 5110 screen driver by github user evovch
- draw lines, circles, squares, bitmaps (includes python script to convert png's), see LCDGraphics.c, builds on the lcd_pcd8544_ll.c low level driver, also uses the existing text drawing functionality in lcd_pcd8544.c

https://github.com/evovch/STM32-LCD_PCD8544


Plays the tetris music by using a timer interrupt to automatically toggle a pin generating a square wave with no involvement from the CPU, and another timer interrupt with an ISR and a longer period between interrupts to sequence the notes that make up the tune

Saves high score to flash with wear leveling, filling up the final 2kb page with high scores and only erasing the page and carrying the high scores over when the page is full (it does this sub-optimally, saving the entire block of six high scores each time when it could only save the latest one, to be revisited)

Future work
- Hardware
  - build a new one with
    - dpad
    - battery
    - enclosure
    - custom pcb, not dev board and prototype board
    - less powerful microcontroller (doesn't need floating point hardware for example)
- software
  - improve wear levelling as described above
  - add more music, make a converter for midi files (probably a python script)
  - add fancier graphics
  - minimise screen re-drawing in a few places, decide whether regions to refresh should be specified manually or automatically at the library / driver level as it currently is (this is inconsistant at the moment)
  - change build scripts to add default high scores to the correct place in flash. Must currently be bootstrapped with temporarry code or manually edited in with STM cube programmer when the chip is first programmed
  - reduce power consumption, i think current delay function used just waits in a loop when it could use some low power sleep mode or be interrupt driven
- hardware and software
  - change to use a d pad
