/*
 * AnalogStick.c
 *
 *  Created on: Jun 29, 2022
 *      Author: James.Marshall
 */


#include "AnalogStick.h"

u16 AD_RES = 0;

void ReadAnalogStick(ADC_HandleTypeDef* adc, u16* outputX, u16* outputY){
		//LCD_PCD8544_clear_ram(&gLcdScreen);
		HAL_ADC_Start(adc);
		// Poll ADC1 Perihperal & TimeOut = 1mSec
		HAL_ADC_PollForConversion(adc, 1);
		// Read The ADC Conversion Result & Map It To PWM DutyCycle
		AD_RES = HAL_ADC_GetValue(adc);
		uint16_t y = AD_RES;

		// Start ADC Conversion
		HAL_ADC_Start(adc);
		// Poll ADC1 Perihperal & TimeOut = 1mSec
		HAL_ADC_PollForConversion(adc, 1);
		// Read The ADC Conversion Result & Map It To PWM DutyCycle
		AD_RES = HAL_ADC_GetValue(adc);
		uint16_t x = AD_RES;
		x &= 0xfff;

		y &= 0xfff;

		*outputX = x;
		*outputY = y;
}

void ReadAnalogStickChange(ADC_HandleTypeDef* adc, i32* outputX, i32* outputY){
	//LCD_PCD8544_clear_ram(&gLcdScreen);
	HAL_ADC_Start(adc);
	// Poll ADC1 Perihperal & TimeOut = 1mSec
	HAL_ADC_PollForConversion(adc, 1);
	// Read The ADC Conversion Result & Map It To PWM DutyCycle
	AD_RES = HAL_ADC_GetValue(adc);
	uint16_t y = AD_RES;

	// Start ADC Conversion
	HAL_ADC_Start(adc);
	// Poll ADC1 Perihperal & TimeOut = 1mSec
	HAL_ADC_PollForConversion(adc, 1);
	// Read The ADC Conversion Result & Map It To PWM DutyCycle
	AD_RES = HAL_ADC_GetValue(adc);
	uint16_t x = AD_RES;
	x &= 0xfff;
	y &= 0xfff;

	const i32 twelveBitMax = 4096;
	*outputX = (x - (twelveBitMax/2));
	*outputY = (y - (twelveBitMax/2));
}
