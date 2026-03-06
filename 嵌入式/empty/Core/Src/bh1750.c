#include "BH1750.h"
#include "stm32f1xx_hal.h"
extern I2C_HandleTypeDef hi2c1;		//引入i2c
extern I2C_HandleTypeDef hi2c2;
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/* I2C2 init function */
void MX_I2C2_Init(void)		//初始化i2c2
{

  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
}
void MX_I2C1_Init(void)		//初始化i2c1
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
}

//void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
//{

//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  if(i2cHandle->Instance==I2C2)
//  {
//  /* USER CODE BEGIN I2C2_MspInit 0 */

//  /* USER CODE END I2C2_MspInit 0 */

//    __HAL_RCC_GPIOB_CLK_ENABLE();
//    /**I2C2 GPIO Configuration
//    PB10     ------> I2C2_SCL
//    PB11     ------> I2C2_SDA
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//    /* I2C2 clock enable */
//    __HAL_RCC_I2C2_CLK_ENABLE();
//  /* USER CODE BEGIN I2C2_MspInit 1 */

//  /* USER CODE END I2C2_MspInit 1 */
//  }
//}

//void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
//{

//  if(i2cHandle->Instance==I2C2)
//  {
//  /* USER CODE BEGIN I2C2_MspDeInit 0 */

//  /* USER CODE END I2C2_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_I2C2_CLK_DISABLE();

//    /**I2C2 GPIO Configuration
//    PB10     ------> I2C2_SCL
//    PB11     ------> I2C2_SDA
//    */
//    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);

//    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11);

//  /* USER CODE BEGIN I2C2_MspDeInit 1 */

//  /* USER CODE END I2C2_MspDeInit 1 */
//  }
//}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
void bh1750_init_all(void){			//初始化四个 BH1750
    MX_I2C2_Init();
    MX_I2C1_Init();
    
	  BH1750_SEND_CMD(&hi2c2, BH1750_ADDR1_WRITE, POWER_ON);// 发送上电指令
    HAL_Delay(5);
    BH1750_SEND_CMD(&hi2c2, BH1750_ADDR1_WRITE,CONT_H_MODE);// 设置连续高精度模式
    BH1750_SEND_CMD(&hi2c2, BH1750_ADDR2_WRITE, POWER_ON);
    HAL_Delay(5);
    BH1750_SEND_CMD(&hi2c2, BH1750_ADDR2_WRITE,CONT_H_MODE);
	  BH1750_SEND_CMD(&hi2c1, BH1750_ADDR3_WRITE, POWER_ON);
    HAL_Delay(5);
    BH1750_SEND_CMD(&hi2c1, BH1750_ADDR3_WRITE,CONT_H_MODE);
	  BH1750_SEND_CMD(&hi2c1, BH1750_ADDR4_WRITE, POWER_ON);
    HAL_Delay(5);
    BH1750_SEND_CMD(&hi2c1, BH1750_ADDR4_WRITE,CONT_H_MODE);
}
//bh1750发送指令函数
uint8_t BH1750_SEND_CMD(I2C_HandleTypeDef*hi2c,uint8_t addr,BH1750_MODE cmd)
{
  return HAL_I2C_Master_Transmit(hi2c,addr,(uint8_t*)&cmd, 1,0xffff);}
//bh1750读取数据函数
uint8_t BH1750_READ_DAT(I2C_HandleTypeDef*hi2c,uint8_t addr,uint8_t* dat)
{
	return HAL_I2C_Master_Receive(hi2c,addr,dat,2,0xffff);
}
//数据转换函数，将传感器数据转化为光照度单位lx
uint16_t BH1750_DAT_TO_LX(uint8_t* dat)
{
	uint16_t lx =0;
	lx=dat[0];
	lx <<=8;
	lx +=dat[1];
	lx=(int) (lx/1.2);
	return lx;
}