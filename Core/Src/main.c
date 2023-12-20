/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LCD2004.h"
#include "Keypad4X4.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
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
TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
extern char key;
char user[]="00000";
char pass[]="12345";
int i=0;
int trail=0;
int lockdown=0;
int mul=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char* intTostr(int num)
{
    char* str=(char*)malloc(3*sizeof(char));
    if (num>99)
        {
            char i1=48+((num%100)%10);
            char i2=48+((num%100)/10);
            char i3=48+(num/100);
            str[0]=i3;
            str[1]=i2;
            str[2]=i1;
            str[3]='\0';
        }
        else if (num>9)
        {
            char i1=48+(num%10);
            char i2=48+(num/10);
            str[0]=i2;
            str[1]=i1;
            str[2]='\0';
        }
        else
        {
            char i=48+num;
            str[0]=i;
            str[1]='\0';
        }
    return str;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim1);
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,0);
  memset(user, '\0', sizeof(user));
  lcd_init();
  lcd_put_cur(0,0);
  lcd_send_string("Enter Pin");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
key=Get_Key();
HAL_Delay(250);
while(key!='D')
{
	if (key!='*'){
	user[i]=key;
	lcd_put_cur(1,i);
	lcd_send_string(&key);
	key=Get_Key();
	HAL_Delay(250);
	}
	if(key=='*')
	{
		lcd_put_cur(1,i);
		lcd_send_string(" ");
		user[i]='\0';
		key=Get_Key();
		HAL_Delay(250);
		i-=1;
	}
	if (key!='*')
	i+=1;
}
if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==0)
{
	if (!strcmp(user,pass))
	{
		lcd_clear();
		lcd_put_cur(0,0);
		lcd_send_string("Welcome");
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,1);
		HAL_Delay(3000);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,0);
		key=Get_Key();
		while (key!='D')
			key=Get_Key();
		if (key=='D')
		{
			lcd_clear();
			lcd_put_cur(0,0);
			lcd_send_string("Enter PIN");
			lcd_put_cur(0,0);
			memset(user, '\0', sizeof(user));
			i=0;
			trail=0;
			lockdown=0;
		}
	}
	else
	{
		trail+=1;
		if(trail<3)
		{
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,1);
			lcd_clear();
			lcd_put_cur(0,0);
			lcd_send_string("Incorrect Pin");
			lcd_put_cur(1,0);
			lcd_send_string("Try Again");
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,1);
			HAL_Delay(500);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,0);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,0);
			lcd_clear();
			lcd_put_cur(0,0);
			lcd_send_string("Enter PIN");
			lcd_put_cur(0,0);
			memset(user, '\0', sizeof(user));
			i=0;
		}
		else
		{
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,1);
			lockdown+=1;
			if (lockdown<3){
			lcd_clear();
			lcd_put_cur(0,0);
			lcd_send_string("Try Again After:");
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,1);
			HAL_Delay(500);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,0);
			HAL_Delay(500);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,1);
			HAL_Delay(500);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,0);
			HAL_Delay(500);


			for (int j=10*(lockdown);j>0;j--)
			{
				lcd_put_cur(1,0);
				lcd_send_string(intTostr(j));
				if(j>9)
				lcd_put_cur(1,2);
				else
				lcd_put_cur(1,1);
				lcd_send_string(" Seconds ");
				HAL_Delay(1000);
			}
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,0);
			lcd_clear();
			lcd_put_cur(0,0);
			lcd_send_string("Enter PIN");
			lcd_put_cur(0,0);
			memset(user, '\0', sizeof(user));
			i=0;
			trail=0;
			}
			else
			{
				lcd_clear();
				lcd_put_cur(0,0);
				lcd_send_string("Entered");
				lcd_put_cur(1,0);
				lcd_send_string("Too Many Wrong Pins");
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,1);
				HAL_Delay(500);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,0);
				HAL_Delay(500);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,1);
				HAL_Delay(500);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,0);
				HAL_Delay(500);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,1);
				HAL_Delay(500);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,0);
				HAL_Delay(500);
				HAL_Delay(2000);
				lcd_clear();
				lcd_put_cur(0,0);
				lcd_send_string("Entering");
				lcd_put_cur(1,0);
				lcd_send_string("Lockdown Mode");
				lcd_put_cur(2,0);
				lcd_send_string("Reset from Mobile");
				HAL_Delay(2000);
				while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)!=1)
				{
					lcd_clear();
					lcd_put_cur(0,0);
					lcd_send_string("Lockdown Mode Since");
					HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,1);
					HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,1);
					for (int j=0;;j++)
					{
						if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==1)
							break;
						lcd_put_cur(1,0);
						if (j!=60)
						{
							lcd_send_string(intTostr(j));
							if(j>99)
							lcd_put_cur(1,3);
							else if(j>9)
							lcd_put_cur(1,2);
							else
							lcd_put_cur(1,1);
						}
						lcd_send_string(" Seconds ");
						if (j==60)
						{
							j=0;
							mul+=1;
							lcd_put_cur(2,0);
							lcd_send_string(intTostr(mul));
							if(mul>99)
							lcd_put_cur(2,3);
							else if(mul>9)
							lcd_put_cur(2,2);
							else
							lcd_put_cur(2,1);
							lcd_send_string(" Minutes");
						}
						if (j==0)
						{
							lcd_put_cur(1,0);
							lcd_send_string("0 Seconds ");
						}
						if (mul==0)
						{
							lcd_put_cur(2,0);
							lcd_send_string("0 Minutes");
						}

						HAL_Delay(1000);
						lcd_put_cur(3,0);
						lcd_send_string("Reset from Mobile");
					}

				}
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,0);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,0);
				lcd_clear();
				lcd_put_cur(0,0);
				lcd_send_string("Enter PIN");
				lcd_put_cur(0,0);
				memset(user, '\0', sizeof(user));
				i=0;
				trail=0;
				lockdown=0;
				mul=0;
			}
		}
	}
}
else
{
	lcd_clear();
	lcd_put_cur(0,0);
	lcd_send_string("Entering");
	lcd_put_cur(1,0);
	lcd_send_string("Pin RESET Mode");
	lcd_put_cur(2,0);
	lcd_send_string("Please Wait");
	HAL_Delay(1000);
	lcd_clear();
	lcd_put_cur(0,0);
	lcd_send_string("Enter New Pin");
	i=0;
	key=Get_Key();
	HAL_Delay(250);
	memset(pass, '\0', sizeof(pass));
	while(i<5)
	{
		pass[i]=key;
		lcd_put_cur(1,i);
		lcd_send_string(&key);
		i+=1;
		key=Get_Key();
		HAL_Delay(250);
	}
	lcd_clear();
	lcd_put_cur(0,0);
	lcd_send_string("Enter PIN");
	lcd_put_cur(0,0);
	memset(user, '\0', sizeof(user));
	i=0;
	trail=0;
	lockdown=0;
}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_TIM1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 72-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0xffff-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_4|GPIO_PIN_5|KC0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|KC3_Pin
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, KC2_Pin|KC1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC0 PC4 PC5 KC0_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_4|GPIO_PIN_5|KC0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PC1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : KR0_Pin KR1_Pin KR2_Pin */
  GPIO_InitStruct.Pin = KR0_Pin|KR1_Pin|KR2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 KC3_Pin
                           PB11 PB12 PB13 PB14
                           PB15 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|KC3_Pin
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : KC2_Pin KC1_Pin */
  GPIO_InitStruct.Pin = KC2_Pin|KC1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : KR3_Pin */
  GPIO_InitStruct.Pin = KR3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(KR3_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
