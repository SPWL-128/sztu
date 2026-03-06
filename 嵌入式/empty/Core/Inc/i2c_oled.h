/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.h
  * @brief   This file contains all the function prototypes for
  *          the i2c.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_OLED_H__
#define __I2C_OLED_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
extern I2C_HandleTypeDef hi2c2;
/* USER CODE BEGIN Includes */
extern const unsigned char F6x8[][6];
extern const unsigned char F8X16[];
extern unsigned char sztu[12][32];
extern const unsigned char gImage_sztu[512];


void MX_I2C_Init(void);
void oled_full(uint8_t data);
void oled_init(void);
void oled_display_char(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);
void oled_show_string(uint8_t x, uint8_t y, char ch[], uint8_t TextSize);
void oled_clear();
void OLED_ShowCHinese_pointer(uint8_t x,uint8_t y,uint8_t no,unsigned char add[][32]);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,const unsigned char BMP[]);
#define display_sztu OLED_ShowCHinese_pointer(2,3,6,sztu);
#define display_sztu_logo OLED_DrawBMP(32, 0, 96, 8,gImage_sztu);
#endif

