/*
 * app.c
 *
 *  Created on: Feb 4, 2025
 *      Author: An SangHyun
 */

#include "app.h"


extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim11;

void delay_us(uint16_t time) {
	htim11.Instance->CNT = 0;
	while(htim11.Instance->CNT < time);
}


int timeoutDoor;
void SystickCallback(){
	if(timeoutDoor > 0) timeoutDoor--;
}

void app(){
	// 초음파 초기화
	initUltrasonic(&htim3);
	// 타이머 초기화
	HAL_TIM_Base_Start(&htim11);

	while(1){
		// 초음파 값 읽기
		uint16_t distance = getDistance();
		printf("%d\n", distance);

		//값비교
		if(distance <= 5) {
			timeoutDoor = 50000;
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 1);

		}
		if(timeoutDoor == 0){
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);
		}
		HAL_Delay(100);
	}
}

