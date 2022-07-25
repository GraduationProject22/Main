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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "irencoder.h"
#include "imu.h"
#include "uartRingBuffer.h"
#include "NMEA.h"
#include "analysis.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
extern char bufferRX[RX_SIZE];
extern char bufferTX[TX_SIZE];
char RMC[100];
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* Ultra Sonic Forward Variables */
#define FORWARD_TRIG_PIN GPIO_PIN_9
#define FORWARD_TRIG_PORT GPIOA
#define FORWARD_ECHO_PIN GPIO_PIN_8
#define FORWARD_ECHO_PORT GPIOA
uint32_t FWpMillis;
uint32_t FWValue1 = 0;
uint32_t FWValue2 = 0;
int16_t FWDistance= 0;  // cm

/* Ultra Sonic Backword Variables */
#define BACKWARD_TRIG_PIN GPIO_PIN_15
#define BACKWARD_TRIG_PORT GPIOB
#define BACKWARD_ECHO_PIN GPIO_PIN_14
#define BACKWARD_ECHO_PORT GPIOB
uint32_t BWpMillis;
uint32_t BWValue1 = 0;
uint32_t BWValue2 = 0;
int16_t BWDistance= 0;  // cm

#define MIN_DISTANCE 30
#define CAR_LENGTH 16
uint8_t FW[1] = {'F'};
uint8_t BW[1] = {'B'};
uint8_t safe[1] = {'T'};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void ultraForward()
{
    HAL_GPIO_WritePin(FORWARD_TRIG_PORT, FORWARD_TRIG_PIN, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while (__HAL_TIM_GET_COUNTER (&htim1) < 10);  // wait for 10 us
    HAL_GPIO_WritePin(FORWARD_TRIG_PORT, FORWARD_TRIG_PIN, GPIO_PIN_RESET);  // pull the TRIG pin low

	FWpMillis = HAL_GetTick(); // used this to avoid infinite while loop  (for timeout)
	// wait for the echo pin to go high
	while (!(HAL_GPIO_ReadPin (FORWARD_ECHO_PORT, FORWARD_ECHO_PIN)) && FWpMillis + 10 >  HAL_GetTick());
	FWValue1 = __HAL_TIM_GET_COUNTER (&htim1);

	FWpMillis = HAL_GetTick(); // used this to avoid infinite while loop (for timeout)
	// wait for the echo pin to go low
	while ((HAL_GPIO_ReadPin (FORWARD_ECHO_PORT, FORWARD_ECHO_PIN)) && FWpMillis + 50 > HAL_GetTick());
	FWValue2 = __HAL_TIM_GET_COUNTER (&htim1);

	FWDistance = (FWValue2-FWValue1)* 0.034/2;
}

void ultraBackward()
{
    HAL_GPIO_WritePin(BACKWARD_TRIG_PORT, BACKWARD_TRIG_PIN, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while (__HAL_TIM_GET_COUNTER (&htim1) < 10);  // wait for 10 us
    HAL_GPIO_WritePin(BACKWARD_TRIG_PORT, BACKWARD_TRIG_PIN, GPIO_PIN_RESET);  // pull the TRIG pin low

	BWpMillis = HAL_GetTick(); // used this to avoid infinite while loop  (for timeout)
	// wait for the echo pin to go high
	while (!(HAL_GPIO_ReadPin (BACKWARD_ECHO_PORT, BACKWARD_ECHO_PIN)) && BWpMillis + 10 >  HAL_GetTick());
	BWValue1 = __HAL_TIM_GET_COUNTER (&htim1);

	BWpMillis = HAL_GetTick(); // used this to avoid infinite while loop (for timeout)
	// wait for the echo pin to go low
	while ((HAL_GPIO_ReadPin (BACKWARD_ECHO_PORT, BACKWARD_ECHO_PIN)) && BWpMillis + 50 > HAL_GetTick());
	BWValue2 = __HAL_TIM_GET_COUNTER (&htim1);

	BWDistance = (BWValue2-BWValue1)* 0.034/2;
}

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

  // pull the TRIG pin low

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_USART2_UART_Init();
  MX_TIM4_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  //HAL_Copmass_Init(&hi2c2);
  Ringbuf_init(); //GPS
  HAL_TIM_Base_Start(&htim1); //Ultra Sonic
  HAL_GPIO_WritePin(FORWARD_TRIG_PORT, FORWARD_TRIG_PIN, GPIO_PIN_RESET);
  HAL_Delay(500);
  HAL_IREncoder_Start(&htim2, &htim3); //Encoder
  HAL_IMU_Init(&hi2c1); //IMU
  HAL_Delay(2000);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


	  if(Wait_for("RMC") == 1)
	  {
	  	 Copy_upto("*", RMC);
	  	 getGPSData();

	  }
	  resetBuffersIndexes();
	  ultraForward();
	  ultraBackward();
	  if((FWDistance <= MIN_DISTANCE)||(BWDistance - CAR_LENGTH <= MIN_DISTANCE))
	  {
		  HAL_UART_Transmit(&huart3, &FW, 1, 0xFF);
		  sendWarning();
	  }
	  else
	  		  HAL_UART_Transmit(&huart3, &safe, 1, 0xFF);
	  HAL_IMU_Read(&hi2c1, &himu);
	  //HAL_Compass_Get(&hi2c2, &hcompass);
	  //HAL_UART_Receive(&huart2, bufferRX, sizeof(bufferRX), 0xFFFF);

	  //getNeighborData();
	  getCarData();
	  //analysis();
	  generateTransmitBuffer();
	  HAL_UART_Transmit(&huart2, bufferTX, sizeof(bufferTX), 0xFFFF);
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
