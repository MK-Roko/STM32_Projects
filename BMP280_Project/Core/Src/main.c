/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include <stdio.h>
#include <string.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  uint8_t data;
  char msg[60];

  // Read id reg
  uint8_t id_reg_read = 0xD0 | 0x80;
  // Pull CS low
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  HAL_SPI_Transmit(&hspi1, &id_reg_read, 1, HAL_MAX_DELAY);
  HAL_SPI_Receive(&hspi1, &data, 1, HAL_MAX_DELAY);

  // pull CS high
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

  sprintf(msg,"Register ID: 0x%X \r\n", data);
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

  // reading compensation parameters
  uint8_t param_buff[24];

  //read 0x88
  uint8_t param_data = 0x88 | 0x80;
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, &param_data, 1, HAL_MAX_DELAY);
  HAL_SPI_Receive(&hspi1,param_buff, 24, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

  typedef struct param_reg{
	  uint16_t dig_T1;
	  int16_t dig_T2;
	  int16_t dig_T3;
	  uint16_t dig_P1;
	  int16_t dig_P2;
	  int16_t dig_P3;
	  int16_t dig_P4;
	  int16_t dig_P5;
	  int16_t dig_P6;
	  int16_t dig_P7;
	  int16_t dig_P8;
	  int16_t dig_P9;
  } BMP280_CalibData;

  BMP280_CalibData calib_data;

  calib_data.dig_T1 = (uint16_t)(param_buff[1] << 8 | param_buff[0]);
  calib_data.dig_T2 = (int16_t)(param_buff[3] << 8 | param_buff[2]);
  calib_data.dig_T3 = (int16_t)(param_buff[5] << 8 | param_buff[4]);
  calib_data.dig_P1 = (uint16_t)(param_buff[7] << 8 | param_buff[6]);
  calib_data.dig_P2 = (int16_t)(param_buff[9] << 8 | param_buff[8]);
  calib_data.dig_P3 = (int16_t)(param_buff[11] << 8 | param_buff[10]);
  calib_data.dig_P4 = (int16_t)(param_buff[13] << 8 | param_buff[12]);
  calib_data.dig_P5 = (int16_t)(param_buff[15] << 8 | param_buff[14]);
  calib_data.dig_P6 = (int16_t)(param_buff[17] << 8 | param_buff[16]);
  calib_data.dig_P7 = (int16_t)(param_buff[19] << 8 | param_buff[18]);
  calib_data.dig_P8 = (int16_t)(param_buff[21] << 8 | param_buff[20]);
  calib_data.dig_P9 = (int16_t)(param_buff[23] << 8 | param_buff[22]);


  char data_msg[50];

  sprintf(data_msg, "Calibration data T1 = %u\r\n",calib_data.dig_T1);
  HAL_UART_Transmit(&huart2, (uint8_t*)data_msg, strlen(data_msg), HAL_MAX_DELAY);

  sprintf(data_msg, "Calibration data T2 = %d\r\n",calib_data.dig_T2);
  HAL_UART_Transmit(&huart2, (uint8_t*)data_msg, strlen(data_msg), HAL_MAX_DELAY);

  sprintf(data_msg, "Calibration data T3 = %d\r\n",calib_data.dig_T3);
  HAL_UART_Transmit(&huart2, (uint8_t*)data_msg, strlen(data_msg), HAL_MAX_DELAY);

  sprintf(data_msg, "Calibration data P1 = %u\r\n",calib_data.dig_P1);
  HAL_UART_Transmit(&huart2, (uint8_t*)data_msg, strlen(data_msg), HAL_MAX_DELAY);

  sprintf(data_msg, "Calibration data P2 = %d\r\n",calib_data.dig_P2);
  HAL_UART_Transmit(&huart2, (uint8_t*)data_msg, strlen(data_msg), HAL_MAX_DELAY);

  sprintf(data_msg, "Calibration data P3 = %d\r\n",calib_data.dig_P3);
  HAL_UART_Transmit(&huart2, (uint8_t*)data_msg, strlen(data_msg), HAL_MAX_DELAY);

  sprintf(data_msg, "Calibration data P4 = %d\r\n",calib_data.dig_P4);
  HAL_UART_Transmit(&huart2, (uint8_t*)data_msg, strlen(data_msg), HAL_MAX_DELAY);

  sprintf(data_msg, "Calibration data P5 = %d\r\n",calib_data.dig_P5);
  HAL_UART_Transmit(&huart2, (uint8_t*)data_msg, strlen(data_msg), HAL_MAX_DELAY);

  sprintf(data_msg, "Calibration data P6 = %d\r\n",calib_data.dig_P6);
  HAL_UART_Transmit(&huart2, (uint8_t*)data_msg, strlen(data_msg), HAL_MAX_DELAY);

  sprintf(data_msg, "Calibration data P7 = %d\r\n",calib_data.dig_P7);
  HAL_UART_Transmit(&huart2, (uint8_t*)data_msg, strlen(data_msg), HAL_MAX_DELAY);

  sprintf(data_msg, "Calibration data P8 = %d\r\n",calib_data.dig_P8);
  HAL_UART_Transmit(&huart2, (uint8_t*)data_msg, strlen(data_msg), HAL_MAX_DELAY);

  sprintf(data_msg, "Calibration data P9 = %d\r\n",calib_data.dig_P9);
  HAL_UART_Transmit(&huart2, (uint8_t*)data_msg, strlen(data_msg), HAL_MAX_DELAY);

// RAW PRESSURE MEASUREMENT

  //write to ctrl meas to enable measurements
  uint8_t ctrl_reg = 0xF4 & 0x7F;
  uint8_t ctrl_reg_setting = 0x27;

  sprintf(msg,"Control Measurement Register: 0x%X \r\n", ctrl_reg);
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, &ctrl_reg, 1, HAL_MAX_DELAY);
  HAL_SPI_Transmit(&hspi1, &ctrl_reg_setting, 1, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

  HAL_Delay(10);

  // read 0xF7 press reg
  uint8_t press_reg = 0xF7 | 0x80;
  uint8_t press_buff[6];
  uint32_t reg_press;
  uint32_t reg_temp;

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, &press_reg, 1, HAL_MAX_DELAY);
  HAL_SPI_Receive(&hspi1, press_buff, 6, HAL_MAX_DELAY);
  // pull CS high
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

  // 0xF7 - 0xF9 Press, 0xFA - 0xFC temp
  reg_press = ((uint32_t)press_buff[0] << 12) |
		  	  (uint32_t)(press_buff[1] << 4) |
			  (uint32_t)(press_buff[2] >> 4);

  reg_temp =  (uint32_t)(press_buff[3] << 12) |
		  	  (uint32_t)(press_buff[4] << 4) |
			  (uint32_t)(press_buff[5] >> 4);

  sprintf(data_msg, "Raw Press = %lu, Raw Temp = %lu\r\n", reg_press, reg_temp);
  HAL_UART_Transmit(&huart2, (uint8_t*)data_msg, strlen(data_msg), HAL_MAX_DELAY);

  // Temperature compensation - Returns temp in Deg, T= 0.01. t_fine carries fine temp as global value
  int32_t t_fine;
  int32_t var1, var2, T;
  char comp_value[100];


  var1 = ((((int32_t)reg_temp >> 3) - ((int32_t)calib_data.dig_T1 << 1)) * ((int32_t)calib_data.dig_T2)) >> 11;
  var2 = (((((int32_t)reg_temp >> 4) - ((int32_t)calib_data.dig_T1)) * (((int32_t)reg_temp >> 4) - ((int32_t)calib_data.dig_T1))) >> 12) *  ((int32_t)calib_data.dig_T3) >> 14;

  t_fine = var1 + var2;
  T = (t_fine * 5 + 128) >> 8;
  float temp = T / 100.0f;
  sprintf(comp_value, "Temp = %.2f C\r\n", temp);
  HAL_UART_Transmit(&huart2, (uint8_t*)comp_value, strlen(comp_value), HAL_MAX_DELAY);

// Pressure compensation
  int64_t var1_p, var2_p, p;

  var1_p = ((int64_t)t_fine) - 128000;
  var2_p = var1_p * var1_p * (int64_t)calib_data.dig_P6;
  var2_p = var2_p + ((var1_p * (int64_t)calib_data.dig_P5) << 17);
  var2_p = var2_p + (((int64_t)calib_data.dig_P4) << 35);
  var1_p = ((var1_p * var1_p * (int64_t)calib_data.dig_P3) >> 8) + ((var1_p * (int64_t)calib_data.dig_P2) << 12);
  var1_p = (((((int64_t)1) << 47) + var1_p) * ((int64_t)calib_data.dig_P1)) >> 33;

  if(var1_p == 0){
	  //return zero
  };

  p = 1048576 - reg_press;
  p = (((p << 31) - var2_p) * 3125) / var1_p;
  var1_p = (((int64_t)calib_data.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
  var2_p = (((int64_t)calib_data.dig_P8) * p) >> 19;
  p = ((p + var1_p + var2_p) >> 8) + (((int64_t)calib_data.dig_P7) << 4);

  sprintf(comp_value, "Pressure = %ld\r\n", (long)p/256);
  HAL_UART_Transmit(&huart2, (uint8_t*)comp_value, strlen(comp_value), HAL_MAX_DELAY);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
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
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
#ifdef USE_FULL_ASSERT
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
