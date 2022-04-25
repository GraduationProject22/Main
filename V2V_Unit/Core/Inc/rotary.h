#ifndef __ROTARY_H__
#define __ROTARY_H__

/*Exported types*/

typedef struct
{
	uint16_t 	RPM;		/*16-bit variable contains the rounds per minutes (RPM)*/
	uint16_t 	Pulses;		/*16-bit variable contains the number of pulses generated from the encoder*/
	int8_t		Dir;		/*8-bit variable contains the direction of the movement (CW, CCW, or STILL)*/
}ROTARY_TypeDef;

/*End of exported types*/


/*Exported constants*/

#define		ROTARY_ENCODER_CW			0xFFUL
#define 	ROTARY_ENCODER_CCW			0x01UL
#define		ROTARY_ENCODER_STILL		0x00UL

#define		ROTARY_READ_DATA_CONT		0x00UL
#define		ROTARY_READ_DATA_ONCE		0x01UL

/*End of exported constants*/


/*Private defines*/

extern 	ROTARY_TypeDef		hrotary;

/*End of private defines*/


/*Prototypes*/

/*This function is to start encoder and update data continuously.*/
void HAL_ROTARY_Start(TIM_HandleTypeDef* Encoder,TIM_HandleTypeDef* Timer,uint32_t APB_Timer_Clock);

/*This function is to start the encoder only one time.*/
void HAL_ROTARY_Read_Once(TIM_HandleTypeDef* Encoder,TIM_HandleTypeDef* Timer,uint32_t APB_Timer_Clock);

/*This function is to stop the encoder.*/
void HAL_ROTARY_Stop(TIM_HandleTypeDef* Encoder,TIM_HandleTypeDef* Timer);

/*End of prototypes*/

#endif
