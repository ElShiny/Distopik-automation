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

#include "stm32c0xx_ll_tim.h"
#include "stm32c0xx_ll_usart.h"
#include "stm32c0xx_ll_rcc.h"
#include "stm32c0xx_ll_system.h"
#include "stm32c0xx_ll_gpio.h"
#include "stm32c0xx_ll_exti.h"
#include "stm32c0xx_ll_bus.h"
#include "stm32c0xx_ll_cortex.h"
#include "stm32c0xx_ll_utils.h"
#include "stm32c0xx_ll_pwr.h"
#include "stm32c0xx_ll_dma.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define IS3_ADR 0x34<<1
#define I2C_WRITE 0
#define I2C_READ 1

static const uint8_t led_adr_arr[]={0x01, 0x11, 0x21, 0x31, 0x41, 0x51,
									0x04, 0x14, 0x24, 0x34, 0x44, 0x54,
									0x07, 0x17, 0x27, 0x37, 0x47, 0x57,
									0x0A, 0x1A, 0x2A, 0x3A, 0x4A, 0x5A,
									0x0D, 0x1D, 0x2D, 0x3D, 0x4D, 0x5D};
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
#define ACE_EN_Pin LL_GPIO_PIN_15
#define ACE_EN_GPIO_Port GPIOC
#define INT_Pin LL_GPIO_PIN_0
#define INT_GPIO_Port GPIOA
#define USR_BTN_Pin LL_GPIO_PIN_5
#define USR_BTN_GPIO_Port GPIOA
#define ACE8_Pin LL_GPIO_PIN_6
#define ACE8_GPIO_Port GPIOA
#define ACE7_Pin LL_GPIO_PIN_7
#define ACE7_GPIO_Port GPIOA
#define ACE6_Pin LL_GPIO_PIN_0
#define ACE6_GPIO_Port GPIOB
#define ACE5_Pin LL_GPIO_PIN_1
#define ACE5_GPIO_Port GPIOB
#define ACE1_Pin LL_GPIO_PIN_8
#define ACE1_GPIO_Port GPIOA
#define ACE2_Pin LL_GPIO_PIN_6
#define ACE2_GPIO_Port GPIOC
#define ACE3_Pin LL_GPIO_PIN_11
#define ACE3_GPIO_Port GPIOA
#define ACE4_Pin LL_GPIO_PIN_12
#define ACE4_GPIO_Port GPIOA
#define TP2_Pin LL_GPIO_PIN_6
#define TP2_GPIO_Port GPIOB
#define TP1_Pin LL_GPIO_PIN_7
#define TP1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
