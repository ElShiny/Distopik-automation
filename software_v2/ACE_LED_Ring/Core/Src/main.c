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
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mb.h"
#include "port.h"
#include "mbport.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern USHORT   usRegInputStart;
extern USHORT   usRegInputBuf[REG_INPUT_NREGS];


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_TIM16_Init();
  /* USER CODE BEGIN 2 */
	//HAL_TIM_Base_Start_IT(&htim16);
	//HAL_TIM_Base_Start(&htim16);

  //add modbus









  uint8_t arr[2] = {0xCF, 0xAE};


  while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr, 2, 100) == HAL_BUSY){}
  uint8_t arr1[2] = {0xA0, 0x01};
  while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr1, 2, 100) == HAL_BUSY){}
  uint8_t arr2[2] = {0xA1, 30};
  while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr2, 2, 100) == HAL_BUSY){}

	for(int a = 0x90; a < 0x9F; a++){
		uint8_t arr3[2] = {a, 60};
		while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr3, 2, 100) == HAL_BUSY){}
	}

	for(int i = 0 ; i<30; i++){
	  uint8_t arr5[4] = {led_adr_arr[i], 0,255,100};
	  while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr5, 4, 100) == HAL_BUSY){}
	}

	//__disable_irq();
	//Error_Handler();
	if(eMBInit(MB_RTU, 0x01, 0, 115200, MB_PAR_NONE) != MB_ENOERR )Error_Handler();
	//if(eMBSetSlaveID( 0x1, FALSE, 0, 0 ) != MB_ENOERR )Error_Handler();
	//if(eMBSetSlaveID(ucSlaveID, xIsRunning, pucAdditional, usAdditionalLen))Error_Handler();
	__enable_irq();
	if(eMBEnable() != MB_ENOERR)Error_Handler();

	//IM16->CNT = 0;
	//HAL_TIM_Base_Start(&htim16);




  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  if(eMBPoll()!=MB_ENOERR)Error_Handler();  /*Modbus poll update in each run*/
        usRegInputBuf[0]++;
        //HAL_Delay(1);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
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
  //__disable_irq();
	for(int i = 0 ; i<30; i++){
	  uint8_t arr6[4] = {led_adr_arr[i], 0,0,255};
	  while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr6, 4, 100) == HAL_BUSY){}
	}
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
