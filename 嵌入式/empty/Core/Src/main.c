/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "gpio.h"
#include "bh1750.h"
#include "i2c_oled.h"
#include "stdio.h"
#include "tim.h"

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
	uint8_t light1[2];			//读取四个光强传感器的原始数据
	uint8_t light2[2];
	uint8_t light3[2];
	uint8_t light4[2];
  double	lt1=0;
	double	lt2=0;
	double	lt3=0;
	double	lt4=0;
  char str1[32];
	char str2[32];
	char str3[32];
	char str4[32];
	char a1[32];
	char a2[32];
	double angle1 = 90;		//两个舵机的初始角度均为90°
  double angle2 = 90;
	double rl_diff = 0;
	double tb_diff = 0;
	double direct1 = 1;			//direct1,2为垂直移动步长
  double direct2 = -1;
	double direct3 = 1;			//direct3，4为水平移动步长
  double direct4 = -1;
	double sum = 0;			//光强总和
	double err = 0;			
	int i,j,sign1,sign2 =1;;		//舵机转向标志位
	

void SystemClock_Config(void);

void Diff_tb(void);
void Diff_rl(void);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  HAL_Init();			//初始化
  SystemClock_Config();
  MX_GPIO_Init();
  PWM_Init();				//MX_TIM3_Init();
  MX_TIM1_Init();
	bh1750_init_all();
  oled_init();
	
  display_sztu_logo;	//显示校徽
	HAL_Delay(1000);
	oled_clear();
	display_sztu;				//显示中文
	HAL_Delay(1000);
	oled_clear();
	GPIO_Init();			//设置PB1，PB5为推挽输出
	

	Set_Two_Servos_Angle(90, 90);  //两个舵机的初始角度均为90°
  HAL_Delay(1000);
	HAL_TIM_Base_Start_IT(&htim1);

  while (1)
  {
	HAL_Delay(10);
	BH1750_READ_DAT(&hi2c2,BH1750_ADDR1_READ,light1);	//读取传感器数据为light
	BH1750_READ_DAT(&hi2c2,BH1750_ADDR2_READ,light2);
	BH1750_READ_DAT(&hi2c1,BH1750_ADDR3_READ,light3);	
	BH1750_READ_DAT(&hi2c1,BH1750_ADDR4_READ,light4);
	lt1=BH1750_DAT_TO_LX(light1);				//将light数据转换为光照度lx并保存为lt
	lt2=BH1750_DAT_TO_LX(light2);
	lt3=BH1750_DAT_TO_LX(light3);
	lt4=BH1750_DAT_TO_LX(light4);
	sprintf(str1,"%5.0f",lt1);					//将lt转化为字符串从而在oled中显示
	sprintf(str2,"%5.0f",lt2);
	sprintf(str3,"%5.0f",lt3);
	sprintf(str4,"%5.0f",lt4);
	oled_show_string(60,1,str1,2);
	oled_show_string(10,1,str2,2);
	oled_show_string(10,3,str3,2);
	oled_show_string(60,3,str4,2);
	


		
	rl_diff = (lt1 + lt4) - (lt3 + lt2);
	tb_diff = (lt1 + lt2) - (lt3 + lt4);
	sum = lt1 + lt2 + lt3 +lt4;
//	if(tb_diff > 10 && angle2>30){
//		angle2 += direct2; //上
//	}else if(tb_diff < -10 && angle2<150){
//	  angle2 += direct1;//下
//	}
//	if(rl_diff > 10 && angle1<181){
//	  angle1 += direct1;//右
//	}else if(rl_diff < -10 && angle1>0){
//	  angle1 += direct2;//左
//	}
//	sprintf(a1,"%5.0f",angle1);
//	sprintf(a2,"%5.0f",angle2);
//  oled_show_string(10,6,a1,2);
//	oled_show_string(60,6,a2,2);
//   HAL_Delay(20);
	 
//	 angle1 = angle1 + 1;
//      	 
//	 
//   Set_Two_Servos_Angle(angle1, angle2);
         
  }
}		
    /* USER CODE END WHILE */

   /* USER CODE BEGIN 3 */
  void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance ==htim1.Instance){
		err = sum/20;			//设置阈值	
		if(tb_diff > err*4 || tb_diff < -err*4)		//差值大于阈值说明光源变动，步长复位，err*4=sum/5
			{ direct1 = 1;
        direct2 = -1;		
		  }
		if(rl_diff > err*4 || rl_diff < -err*4)
			{ direct3 = 1;
        direct4 = -1;		
		  }
		if(direct1 == 0 ||  direct2 == 0){		//有光则追光
		}else{
		  Diff_tb();
		}
		if(direct3 == 0 ||  direct4 == 0){
		}else{		
		  Diff_rl();
		}	
	}
}



/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


void Diff_tb(void)		//上0-180下
{
  
	if(sum < 100){		//100为启动阈值
		
  }else if(tb_diff >= err && angle2>30){	//上大
		if(sign2 ==0){			//标志位1为上大
		  sign2 = 1;
			if(i > 20){
			
	    }else if(i < 20 && direct2 < 0){		//若行程太小则说明进入特殊位置
		    direct2 += 0.2;										//减小步长，精确定位
				direct1 -= 0.2;
	    }	
		  i = 0;			//变向后行程记录初始化
	  }
    i++;			//记录行程
    	
		angle2 += direct2;		//向上调整
		
	}else if(tb_diff <= -err && angle2<150){//下大
		if(sign2 ==1){
		  sign2 = 0;			//标志位0为下大
			if(i > 20){
		
	    }else if((i < 20 && direct1 > 0)){//若行程太小则说明进入特殊位置
		  direct1 -= 0.2;										//减小步长，精确定位
			direct2 += 0.2;
	    }	
		  i = 0;			//变向后行程记录初始化
	  }
    i++;		//记录行程
    	
	  angle2 += direct1;	//向下调整
		
	}  
	
  Set_Servo_Angle(&htim3, TIM_CHANNEL_4, angle2);	//更新角度
}

void Diff_rl(void)			//左0-180右
{
	  if(rl_diff >= err && angle1<180){			//右大
		  if(sign1 ==0){
		  sign1 = 1;			//标志位1为右大
		  
			if(j > 20){
	    }else if(j < 20 && direct3 > 0){	//若行程太小则说明进入特殊位置
		    direct3 -= 0.2;									//减小步长，精确定位
        direct4 += 0.2;				
	    }
			j = 0;
	  }
    j++;			//记录行程
		angle1 += direct3;	//向右调整
		
	}else if(rl_diff <= -err && angle1>0){
		if(sign1 ==1){
		  sign1 = 0;
			if(j > 20){
			
	    }else if(j < 20 && direct4 < 0){
		    direct4 += 0.2;
				direct3 -= 0.2;
	    }	
		  j = 0;
	  }
    j++;
    
	  angle1 += direct4;
		
	}
	Set_Servo_Angle(&htim3, TIM_CHANNEL_2, angle1);	//更新角度
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
