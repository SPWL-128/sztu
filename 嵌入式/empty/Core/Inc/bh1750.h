#ifndef __BH1750_H__
#define __BH1750_H__
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern I2C_HandleTypeDef hi2c2;			//引用i2c
extern I2C_HandleTypeDef hi2c1;
void MX_I2C2_Init(void);						//初始化i2c
void MX_I2C1_Init(void);


#ifdef __cplusplus
}
#endif
#define BH1750_ADDR1_WRITE 0x46			//四个bh1750设备的地址
#define BH1750_ADDR1_READ 0x47
#define BH1750_ADDR2_WRITE 0xB8
#define BH1750_ADDR2_READ 0xB9
#define BH1750_ADDR3_WRITE 0x46  
#define BH1750_ADDR3_READ 0x47
#define BH1750_ADDR4_WRITE 0xB8 
#define BH1750_ADDR4_READ 0xB9
extern I2C_HandleTypeDef hi2c2;
extern I2C_HandleTypeDef hi2c1;
typedef enum
{
 POWER_OFF =0x00,
 POWER_ON = 0x01,
 RESET_BH1750 = 0X07,
 CONT_H_MODE =0x10,
 CONT_H_MODE2 = 0x11,
 CONT_L_MODE =0x13,
 ONCE_H_MODE = 0x20,
 ONCE_H_MODE2 = 0x21,
 ONCE_L_MODE = 0x23,
} BH1750_MODE;

uint8_t BH1750_SEND_CMD(I2C_HandleTypeDef*hi2c,uint8_t addr,BH1750_MODE cmd);			//声明1750读、写函数，转换函数
uint8_t BH1750_READ_DAT(I2C_HandleTypeDef*hi2c,uint8_t addr,uint8_t* DAT);
uint16_t BH1750_DAT_TO_LX(uint8_t* dat);
void bh1750_init_all(void);						//声明初始化1750函数
#endif
