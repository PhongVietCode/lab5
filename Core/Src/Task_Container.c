/*
 * Task_Container.c
 *
 *  Created on: Nov 24, 2023
 *      Author: phongtran
 */
#include "Task_Container.h"

void initTask(){
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 1);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, 1);
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, 1);
	HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, 1);
}
void LED_Blink_500ms(){
	HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
}
void LED_Blink_1000ms(){
	HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
}
void LED_Blink_1500ms(){
	HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
}
void LED_Blink_2000ms(){
	HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
}
void LED_Blink_2500ms(){
	HAL_GPIO_TogglePin(LED5_GPIO_Port, LED5_Pin);
}
