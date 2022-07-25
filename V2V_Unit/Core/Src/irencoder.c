/*Includes*/

#include "main.h"
#include "irencoder.h"

/*End of includes*/

/*Private defines*/

IRENCODER_TypeDef		hirencoder;
TIM_HandleTypeDef 		*hcounter;
TIM_HandleTypeDef 		*htimer;

/*End of private defines*/

/*Functions*/

/*This function starts the IR encoder*/
void HAL_IREncoder_Start(TIM_HandleTypeDef *Counter, TIM_HandleTypeDef *Timer)
{
	uint32_t Dividend = 0;
	uint32_t Quotient = 0;
	uint32_t Remainder = 0;
	uint16_t Divisor = 0;
	uint8_t Accurecy = 0;

	/*Saving the timers*/
	hcounter = Counter;
	htimer = Timer;

	/*Setting the timer to over flow when IRENCODER_FIXED_TIME_VALUE is elapsed*/
	Dividend = (IRENCODER_TIMER_CLK * IRENCODER_FIXED_TIME_VALUE) - 1;
	 do
	 {
		 if (Dividend == 0)
		 {
			 break;
		 }
		 ++Divisor;
		 Quotient = Dividend / Divisor;
		 /*Checking for overflow*/
		 if (Divisor == 0)
		 {
			 /*Decreasing the accuracy*/
			 Dividend += 1;
		 }
	 }while(!((Quotient <= 65535)&&(Remainder == 0)));

	 Timer->Instance->PSC = Divisor;
	 Timer->Instance->ARR = Quotient;

	/*Starting the timers*/
	HAL_TIM_Base_Start(Counter);
	HAL_TIM_Base_Start_IT(Timer);
}

/*This function is to stop the IR Encoder*/
void HAL_IREncoder_Stop(TIM_HandleTypeDef *Counter, TIM_HandleTypeDef *Timer)
{
	/*Stopping the timers*/
	HAL_TIM_Base_Stop(Counter);
	HAL_TIM_Base_Stop_IT(Timer);
}

/*This function is the callback function when the interrupt occurs*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/*Saving the counts*/
	hirencoder.Pulses = hcounter->Instance->CNT;
	/*Stop the timer and the counter*/
	HAL_TIM_Base_Stop(hcounter);
	HAL_TIM_Base_Stop_IT(htimer);
	/*Calculate the RPM*/
	hirencoder.RPM = (hirencoder.Pulses*1000*60)/(IRENCODER_ONE_CYCLE_PULSES*IRENCODER_FIXED_TIME_VALUE);
	/*Reset the timer and the counter*/
	hcounter->Instance->CNT = 0;
	htimer->Instance->CNT = 0;
	/*Start the timer and the counter*/
	HAL_TIM_Base_Start(hcounter);
	HAL_TIM_Base_Start_IT(htimer);
}

/*End of functions*/

