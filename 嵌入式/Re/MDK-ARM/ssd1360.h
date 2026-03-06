#ifndef __SSD1306_H
#define __SSD1306_H

#include "main.h"

void SSD1306_Init(void);
void SSD1306_Clear(void);
void SSD1306_SetCursor(uint8_t x, uint8_t y);
void SSD1306_WriteString(char *str);
void SSD1306_WriteInt(uint16_t value);

#endif