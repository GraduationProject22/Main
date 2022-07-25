#ifndef __IRENCODER_H__
#define __IRENCODER_H__

/*Includes*/

#include "irencoder_config.h"

/*End of includes*/

/*Exported types*/

typedef struct
{
	uint16_t 	RPM;		/*16-bit variable contains the rounds per minutes (RPM)*/
	uint16_t 	Pulses;		/*16-bit variable contains the number of pulses generated from the encoder*/
}IRENCODER_TypeDef;

/*End of exported types*/

/*Private defines*/

extern 	IRENCODER_TypeDef		hirencoder;

/*End of private defines*/

/*Prototypes*/

/*This function starts the IR encoder*/
void HAL_IREncoder_Start(TIM_HandleTypeDef *Counter, TIM_HandleTypeDef *Timer);

/*This function is to stop the IR Encoder*/
void HAL_IREncoder_Stop(TIM_HandleTypeDef *Counter, TIM_HandleTypeDef *Timer);

/*End of prototypes*/

#endif
