/*Includes*/

#include "main.h"
#include "rotary.h"

/*End of includes*/

/*Private defines*/

ROTARY_TypeDef		hrotary;
TIM_HandleTypeDef	hencoder;
TIM_HandleTypeDef	htimer;
uint8_t				Read_Data;
uint32_t			frequency;
int16_t				Count1 = 0;
int16_t				Count2 = 0;	

/*End of private defines*/


/*Functions*/

/*This function is to start encoder and update data continuously.*/
/*APB_Timer_Clock is in kHz*/
void HAL_ROTARY_Start(TIM_HandleTypeDef* Encoder,TIM_HandleTypeDef* Timer,uint32_t APB_Timer_Clock)
{
	Read_Data = ROTARY_READ_DATA_CONT;
	if((Timer->Init.Prescaler != 0 )&&(Timer->Init.Period != 0))
	{
		frequency = ((APB_Timer_Clock*1000)/Timer->Init.Prescaler)/Timer->Init.Period;
	}
	else if ((Timer->Init.Prescaler = 0 )&&(Timer->Init.Period != 0))
	{
		frequency = (APB_Timer_Clock*1000)/Timer->Init.Period;
	}
	else if ((Timer->Init.Prescaler != 0 )&&(Timer->Init.Period = 0))
	{
		frequency = (APB_Timer_Clock*1000)/Timer->Init.Prescaler;
	}
	else
	{
		frequency = APB_Timer_Clock*1000;
	}
	hencoder = *Encoder;
	htimer = *Timer;
	/*Starting the encoder*/
	HAL_TIM_Encoder_Start(Encoder,TIM_CHANNEL_ALL);
	/*Starting the timer*/
	HAL_TIM_Base_Start_IT(Timer);
}

/*This function is to start the encoder only one time.*/
/*APB_Timer_Clock is in kHz*/
void HAL_ROTARY_Read_Once(TIM_HandleTypeDef* Encoder,TIM_HandleTypeDef* Timer,uint32_t APB_Timer_Clock)
{
	Read_Data = ROTARY_READ_DATA_ONCE;
	if((Timer->Init.Prescaler != 0 )&&(Timer->Init.Period != 0))
	{
		frequency = ((APB_Timer_Clock*1000)/Timer->Init.Prescaler)/Timer->Init.Period;
	}
	else if ((Timer->Init.Prescaler = 0 )&&(Timer->Init.Period != 0))
	{
		frequency = (APB_Timer_Clock*1000)/Timer->Init.Period;
	}
	else if ((Timer->Init.Prescaler != 0 )&&(Timer->Init.Period = 0))
	{
		frequency = (APB_Timer_Clock*1000)/Timer->Init.Prescaler;
	}
	else
	{
		frequency = APB_Timer_Clock*1000;
	}
	hencoder = *Encoder;
	htimer = *Timer;
	/*Starting the encoder*/
	HAL_TIM_Encoder_Start(Encoder,TIM_CHANNEL_ALL);
	/*Starting the timer*/
	HAL_TIM_Base_Start_IT(Timer);
}

/*This function is to stop the encoder.*/
void HAL_ROTARY_Stop(TIM_HandleTypeDef* Encoder,TIM_HandleTypeDef* Timer)
{
	/*Stopping the encoder*/
	HAL_TIM_Encoder_Stop(Encoder,TIM_CHANNEL_ALL);
	/*Stopping the timer*/
	HAL_TIM_Base_Stop_IT(Timer);
}

/*This function generates the data from the interrupt of the timer*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/*Reading the register of the encoder timer*/
	Count2 = hencoder.Instance->CNT;
	/*Disabling the timer if the data is read once*/
	if (Read_Data == ROTARY_READ_DATA_ONCE)
	{
		HAL_ROTARY_Stop(&hencoder,&htimer);
		htimer.Instance->CNT = 0x00UL;
	}
	/*Checking if the number is negative to detect the direction*/
	if ((Count2 - Count1) < 0)
	{
		hrotary.Pulses = Count1 - Count2;
		hrotary.Dir = ROTARY_ENCODER_CW;
	}
	else if ((Count2 - Count1) > 0)
	{
		hrotary.Pulses = Count2 - Count1;
		hrotary.Dir = ROTARY_ENCODER_CCW;
	}
	else
	{
		hrotary.Pulses = Count2 - Count1;
		hrotary.Dir = ROTARY_ENCODER_STILL;
	}
	/*Calculating the RPM*/
	hrotary.RPM = hrotary.Pulses*frequency*60/80;
	if (Read_Data == ROTARY_READ_DATA_CONT)
	{
		/*Assigning the register value used earlier to be used as start of the next calculation*/
		Count1 = Count2;
	}
	else if (Read_Data == ROTARY_READ_DATA_ONCE)
	{
		/*Reseting the counters*/
		hencoder.Instance->CNT = 0x00UL;
		Count1 = 0;
	}

}

/*End of prototypes*/
