/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
void Left_Forward(void);
void Right_Forward(void);

void Left_Backward(void);
void Right_Backward(void);

void Left_Stop(void);
void Right_Stop(void);

void Forward(void);
void Backward(void);
void Stop(void);
void Left_Rot(void);
void Right_Rot(void);
void Forward_Left_Turn(void);
void Forward_Right_Turn(void);
void Backward_Left_Turn(void);
void Backward_Right_Turn(void);

void Speed_Up(void);
void Speed_Down(void);

void Fixed_Speed(void);

//void Left_Turn_Speed(void);
//void Right_Turn_Speed(void);
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define CAR_LENGTH 16
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t Speed = 1000;		//Start with half speed
uint8_t CTRL = 0;			//Control signal through Bluetooth
uint8_t MSG[4] = {'\0'};
uint8_t distance = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#if 0
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	//if(((CTRL == 'B' || CTRL == 'H'  || CTRL == 'J') && distance == 'B') || ((CTRL == 'F' || CTRL == 'I'  || CTRL == 'G') && distance == 'F'))
	if(distance == 'B' || distance == 'F')
	{
		Stop();
		//		CTRL = 'S';
		//HAL_Delay(2000);
	}
	//distance = 0;
}
#endif
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
	MX_TIM3_Init();
	MX_USART2_UART_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
	//  HAL_UART_Receive_IT(&huart1, &distance, 1);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

		CTRL = 0;

		HAL_UART_Receive(&huart2, &CTRL, 1, 0xFF);
		//#if 1
		HAL_UART_Receive(&huart1, &distance, 1, 0xFF);

		if(!(distance == 'T'))
		{
			Stop();
		}
		else
		{	//#endif
			switch (CTRL)
			{
			case 'S' :
				Stop();
				break;

			case 'F':
				Forward();
				break;

			case 'B':
				Backward();
				break;

			case 'L':
				Left_Rot();
				break;

			case 'R':
				Right_Rot();
				break;

			case 'G':
				Forward_Left_Turn();
				break;

			case 'I':
				Forward_Right_Turn();
				break;

			case 'H':
				Backward_Left_Turn();
				break;

			case 'J':
				Backward_Right_Turn();
				break;

			case 'U':
				Speed_Up();
				sprintf(MSG,"%d\n",Speed);
				HAL_UART_Transmit(&huart2, MSG, 4, HAL_MAX_DELAY);
				break;

			case 'D':
				Speed_Down();
				sprintf(MSG,"%d\n",Speed);
				HAL_UART_Transmit(&huart2, MSG, 4, HAL_MAX_DELAY);
				break;

			case '0':
				Stop();
				break;

			case '1':
				Speed = 100;
				break;

			case '2':
				Speed = 200;
				break;

			case '3':
				Speed = 300;
				break;

			case '4':
				Speed = 400;
				break;

			case '5':
				Speed = 500;
				break;

			case '6':
				Speed = 600;
				break;

			case '7':
				Speed = 700;
				break;

			case '8':
				Speed = 800;
				break;

			case '9':
				Speed = 900;
				break;

			case 'q':
				Speed = 1000;
				break;
			}
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

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV16;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
void Left_Forward(void)
{
	L1_State(GPIO_PIN_RESET);
	L2_State(GPIO_PIN_SET);
}
void Right_Forward(void)
{
	R1_State(GPIO_PIN_RESET);
	R2_State(GPIO_PIN_SET);
}

void Left_Backward(void)
{
	L1_State(GPIO_PIN_SET);
	L2_State(GPIO_PIN_RESET);
}
void Right_Backward(void)
{
	R1_State(GPIO_PIN_SET);
	R2_State(GPIO_PIN_RESET);
}

void Left_Stop(void)
{
	L1_State(GPIO_PIN_RESET);
	L2_State(GPIO_PIN_RESET);
}
void Right_Stop(void)
{
	R1_State(GPIO_PIN_RESET);
	R2_State(GPIO_PIN_RESET);
}

void Forward(void)
{
	Fixed_Speed();
	Left_Forward();
	Right_Forward();
}
void Backward(void)
{
	Fixed_Speed();
	Left_Backward();
	Right_Backward();
}
void Stop(void)
{
	Left_Stop();
	Right_Stop();
}
void Left_Rot(void)
{
	Fixed_Speed();
	Left_Backward();
	Right_Forward();
}
void Right_Rot(void)
{
	Fixed_Speed();
	Left_Forward();
	Right_Backward();
}
void Forward_Left_Turn(void)
{
	Fixed_Speed();
	Left_Stop();
	Right_Forward();
}
void Forward_Right_Turn(void)
{
	Fixed_Speed();
	Right_Stop();
	Left_Forward();
}
void Backward_Left_Turn(void)
{
	Fixed_Speed();
	Left_Stop();
	Right_Backward();
}
void Backward_Right_Turn(void)
{
	Fixed_Speed();
	Right_Stop();
	Left_Backward();
}

void Speed_Up(void)
{
	if (Speed >= 1000)
	{
		Speed = 1000;
	}
	else
	{
		Speed += 100;
	}
	Fixed_Speed();
}
void Speed_Down(void)
{
	if (Speed <= 100)
	{
		Speed = 100;
	}
	else
	{
		Speed -= 100;
	}
	Fixed_Speed();
}
void Fixed_Speed(void)
{
	TIM3->CCR3 = Speed;
	TIM3->CCR4 = Speed;
}
//void Left_Turn_Speed(void)
//{
//	if (Speed >= 1000)
//	{
//		TIM3->CCR4 = Speed - 600;
//	}
//	else
//	{
//		TIM3->CCR3 = Speed + 600;
//	}
//}
//void Right_Turn_Speed(void)
//{
//	if (Speed >= 1000)
//	{
//		TIM3->CCR3 = Speed - 600;
//	}
//	else
//	{
//		TIM3->CCR4 = Speed + 600;
//	}
//}
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
