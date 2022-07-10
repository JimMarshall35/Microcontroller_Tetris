# Microcontroller_Tetris

This is a tetris game made using this microcontroller board:
https://www.st.com/en/evaluation-tools/nucleo-f303k8.html

a nokia 5110 screen, a ps2 analog stick and a button.

The game is capable of storing high scores on flash rom and stores accumulated game play time on rom and uses to seed the pseudorandom number generator.

Like real versions of tetris this has levels with the difficulty increasing - the levels and fall speeds should be the same as in gameboy tetris.

This repo is a project for STM32CubeIDE 1.9.0 and uses the STM HAL and CubeMX gui for autogenerating peripheral setup code
