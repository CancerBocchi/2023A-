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
#include "stm32g4xx_hal.h"

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
#define Vgrid_Pin GPIO_PIN_3
#define Vgrid_GPIO_Port GPIOC
#define Vout_Pin GPIO_PIN_0
#define Vout_GPIO_Port GPIOA
#define Iout_Pin GPIO_PIN_1
#define Iout_GPIO_Port GPIOA
#define Vbus_Pin GPIO_PIN_2
#define Vbus_GPIO_Port GPIOA
#define RES_Pin GPIO_PIN_0
#define RES_GPIO_Port GPIOB
#define DC_Pin GPIO_PIN_2
#define DC_GPIO_Port GPIOB
#define CS_Pin GPIO_PIN_11
#define CS_GPIO_Port GPIOB
#define row_4_Pin GPIO_PIN_10
#define row_4_GPIO_Port GPIOC
#define col_4_Pin GPIO_PIN_11
#define col_4_GPIO_Port GPIOC
#define row_3_Pin GPIO_PIN_12
#define row_3_GPIO_Port GPIOC
#define col_3_Pin GPIO_PIN_2
#define col_3_GPIO_Port GPIOD
#define row_2_Pin GPIO_PIN_3
#define row_2_GPIO_Port GPIOB
#define col_2_Pin GPIO_PIN_4
#define col_2_GPIO_Port GPIOB
#define row_1_Pin GPIO_PIN_5
#define row_1_GPIO_Port GPIOB
#define col_1_Pin GPIO_PIN_6
#define col_1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
