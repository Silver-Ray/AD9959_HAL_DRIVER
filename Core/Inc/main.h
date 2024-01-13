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
#include "stm32f4xx_hal.h"

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
#define LED0_Pin GPIO_PIN_9
#define LED0_GPIO_Port GPIOF
#define LED1_Pin GPIO_PIN_10
#define LED1_GPIO_Port GPIOF
#define SDIO1_Pin GPIO_PIN_8
#define SDIO1_GPIO_Port GPIOD
#define SDIO2_Pin GPIO_PIN_9
#define SDIO2_GPIO_Port GPIOD
#define SDIO3_Pin GPIO_PIN_10
#define SDIO3_GPIO_Port GPIOD
#define PDC_Pin GPIO_PIN_11
#define PDC_GPIO_Port GPIOD
#define RST_Pin GPIO_PIN_14
#define RST_GPIO_Port GPIOD
#define SCLK_Pin GPIO_PIN_0
#define SCLK_GPIO_Port GPIOD
#define CS_Pin GPIO_PIN_1
#define CS_GPIO_Port GPIOD
#define UPDATE_Pin GPIO_PIN_2
#define UPDATE_GPIO_Port GPIOD
#define SDIO0_Pin GPIO_PIN_3
#define SDIO0_GPIO_Port GPIOD
#define PS0_Pin GPIO_PIN_4
#define PS0_GPIO_Port GPIOD
#define PS1_Pin GPIO_PIN_5
#define PS1_GPIO_Port GPIOD
#define PS2_Pin GPIO_PIN_6
#define PS2_GPIO_Port GPIOD
#define PS3_Pin GPIO_PIN_7
#define PS3_GPIO_Port GPIOD
#define KEY1_Pin GPIO_PIN_8
#define KEY1_GPIO_Port GPIOB
#define KEY1_EXTI_IRQn EXTI9_5_IRQn
#define KEY0_Pin GPIO_PIN_9
#define KEY0_GPIO_Port GPIOB
#define KEY0_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
