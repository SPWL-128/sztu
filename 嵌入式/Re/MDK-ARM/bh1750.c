#include "bh1750.h"
#include "i2c.h"
#include <stdint.h>

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;

#define BH1750_POWER_ON       0x01
#define BH1750_RESET          0x07
#define BH1750_CONT_HIGH_RES  0x10   // ????????

void BH1750_Init(void) {
    uint8_t cmd = BH1750_CONT_HIGH_RES;
    // ??? I2C1 ???????
    HAL_I2C_Master_Transmit(&hi2c1, BH1750_ADDR_LOW << 1, &cmd, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Transmit(&hi2c1, BH1750_ADDR_HIGH << 1, &cmd, 1, HAL_MAX_DELAY);
    // ??? I2C2 ???????
    HAL_I2C_Master_Transmit(&hi2c2, BH1750_ADDR_LOW << 1, &cmd, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Transmit(&hi2c2, BH1750_ADDR_HIGH << 1, &cmd, 1, HAL_MAX_DELAY);
}

uint16_t BH1750_Read(I2C_HandleTypeDef *hi2c, uint8_t dev_addr) {
    uint8_t data[2];
    // ??????????????
    HAL_I2C_Master_Receive(hi2c, dev_addr << 1, data, 2, HAL_MAX_DELAY);
    return (data[0] << 8) | data[1];
}

void BH1750_ReadAll(uint16_t *data) {
    // ?????????????(????????????)
    data[0] = BH1750_Read(&hi2c1, BH1750_ADDR_LOW);
    data[1] = BH1750_Read(&hi2c1, BH1750_ADDR_HIGH);
    data[2] = BH1750_Read(&hi2c2, BH1750_ADDR_LOW);
    data[3] = BH1750_Read(&hi2c2, BH1750_ADDR_HIGH);
}