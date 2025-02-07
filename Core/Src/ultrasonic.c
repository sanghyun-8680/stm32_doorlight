/*
 * ultrasonic.c
 *
 *  Created on: Feb 6, 2025
 *      Author: An SangHyun
 */

#include "ultrasonic.h"


TIM_HandleTypeDef *myHtim;

uint32_t valueIC1 = 0;
uint32_t valueIC2 = 0;
uint32_t difference = 0;
bool IsFIrtsCaptured = 0;
bool doneCapture = false;
uint16_t distance = 0;


void initUltrasonic(TIM_HandleTypeDef *htim){
	myHtim = htim;
	HAL_TIM_IC_Start_IT(myHtim, TIM_CHANNEL_1);
}

// input capture interrupt callback, 캡처 핀의 신호 감지시 작동
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
		if(IsFIrtsCaptured == 0){
			// echo의 상승 신호 발생 시각을 valueIC1에 기록
			valueIC1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
			IsFIrtsCaptured = 1;
			// echo의 하강 신호에 대응되는 인터럽트 설정
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		else if(IsFIrtsCaptured == 1){
		 // echo의 하강 신호 발생 시각을 valueIC2에 기록
			valueIC2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
			// valueIC1과 valueIC2의 시간차 계산
			if(valueIC1 < valueIC2){
				difference = valueIC2 - valueIC1;
			}
			else if(valueIC1 > valueIC2){
				difference = (65535 - valueIC1) + valueIC2;
			}
			//거리 계산, 소리의 속도 0.034
			distance = difference * 0.034 / 2;
			IsFIrtsCaptured = 0;
			doneCapture = true;
			// 다음 측정을 위해 초기화
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1); // 인터럽트 끔
		}
	}
}

uint16_t	getDistance() {
	// TRIG 핀에 10us의 펄스를 출력
	HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, 1);
	delay_us(10);
	HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, 0);
	// 인터럽트 활성화
	__HAL_TIM_ENABLE_IT(myHtim, TIM_IT_CC1);
	// 캡처 종료 대기
	doneCapture = false;
	while(doneCapture == false);
	// 값 반환
	return distance;
}
