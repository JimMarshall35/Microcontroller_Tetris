/*
 * AnalogStick.h
 *
 *  Created on: Jun 29, 2022
 *      Author: James.Marshall
 */

#ifndef INC_ANALOGSTICK_H_
#define INC_ANALOGSTICK_H_
#include "BasicTypedefs.h"

#include "stm32f3xx_hal.h"

typedef enum{
	NO_DIRECTION,
	UP,
	DOWN,
	LEFT,
	RIGHT
}ANALOG_STICK_DPAD_RESULT;

void ReadAnalogStick(ADC_HandleTypeDef* adc, u16* outputX, u16* outputY);
void ReadAnalogStickChange(ADC_HandleTypeDef* adc, i32* outputX, i32* outputY);
ANALOG_STICK_DPAD_RESULT EmulateDPad(ADC_HandleTypeDef* adc);
ANALOG_STICK_DPAD_RESULT EmulateDPadReturningXAndYChange(ADC_HandleTypeDef* adc, i32* rXChange, i32* rYChange);
#endif /* INC_ANALOGSTICK_H_ */
