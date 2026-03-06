#include "servo.h"

void Servo_SetAngle(TIM_HandleTypeDef *htim, uint32_t channel, float angle) {
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    // ?????20000,0°??500,180°??2500
    uint16_t ccr = 500 + (uint16_t)(angle * 2000.0f / 180.0f);
    __HAL_TIM_SET_COMPARE(htim, channel, ccr);
}