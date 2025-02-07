/*
 * ultrasonic.h
 *
 *  Created on: Feb 6, 2025
 *      Author: An SangHyun
 */

#ifndef SRC_ULTRASONIC_H_
#define SRC_ULTRASONIC_H_

#include "main.h"

void initUltrasonic(TIM_HandleTypeDef *htim);
uint16_t	getDistance();

#endif /* SRC_ULTRASONIC_H_ */
