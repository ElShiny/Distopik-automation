/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32c0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define INT_Pin GPIO_PIN_0
#define INT_GPIO_Port GPIOA
#define USR_BTN_Pin GPIO_PIN_5
#define USR_BTN_GPIO_Port GPIOA
#define WIPER_Pin GPIO_PIN_7
#define WIPER_GPIO_Port GPIOA
#define MOT_SLEEP_Pin GPIO_PIN_1
#define MOT_SLEEP_GPIO_Port GPIOB
#define MOT1_Pin GPIO_PIN_8
#define MOT1_GPIO_Port GPIOA
#define MOT2_Pin GPIO_PIN_11
#define MOT2_GPIO_Port GPIOA
#define TP2_Pin GPIO_PIN_6
#define TP2_GPIO_Port GPIOB
#define TP1_Pin GPIO_PIN_7
#define TP1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
