#ifndef __SERVO_H
#define __SERVO_H

#include "main.h"

void Servo_SetAngle(TIM_HandleTypeDef *htim, uint32_t channel, float angle);

#endif