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

void ReadAnalogStick(ADC_HandleTypeDef* adc, u16* outputX, u16* outputY);
void ReadAnalogStickChange(ADC_HandleTypeDef* adc, i32* outputX, i32* outputY);

#endif /* INC_ANALOGSTICK_H_ */
