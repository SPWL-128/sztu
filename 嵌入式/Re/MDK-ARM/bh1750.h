#ifndef __BH1750_H
#define __BH1750_H

#include "main.h"

// BH1750 7??? (ADDR???0x46,?VCC?0x5A)
#define BH1750_ADDR_LOW  0x46
#define BH1750_ADDR_HIGH 0x5A

void BH1750_Init(void);
uint16_t BH1750_Read(I2C_HandleTypeDef *hi2c, uint8_t dev_addr);
void BH1750_ReadAll(uint16_t *data);

#endif