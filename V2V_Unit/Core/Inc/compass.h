#ifndef __COMPASS_H__
#define __COMPASS_H__

/*Includes*/

#include "compass_reg.h"
#include "compass_config.h"

/*End of includes*/

/*Exported types*/

typedef struct
{
	int16_t	X_RAW;			/*16-bit variable containing the ADC number representing the magnetic in x-axies*/
	int16_t	Y_RAW;			/*16-bit variable containing the ADC number representing the magnetic in y-axies*/
	
	int16_t X_RAW_CAL;		/*16-bit variable containing the ADC number representing the magnetic in x-axies after calibration*/
	int16_t	Y_RAW_CAL;		/*16-bit variable containing the ADC number representing the magnetic in y-axies after calibration*/
	
	double	Angle_x;		/*The angle between the north and the x-axies*/
	double	Angle_y;		/*The angle between the north and the y-axies*/
	
}Compass_TypeDef;

/*End of exported types*/

/*Private defines*/

extern 	Compass_TypeDef		hcompass;

/*End of private defines*/

/*Exported constants*/

#define MATH_PI		3.141592653589793

#define COMPASS_ADDR		0x3C

#define COMPASS_AVERAGE_SAMPLES_1	0x00UL
#define COMPASS_AVERAGE_SAMPLES_2	0x01UL
#define COMPASS_AVERAGE_SAMPLES_4	0x02UL
#define COMPASS_AVERAGE_SAMPLES_8	0x03UL

#define COMPASS_DATA_OUTPUT_RATE_0_75	0x00UL
#define COMPASS_DATA_OUTPUT_RATE_1_5	0x01UL
#define COMPASS_DATA_OUTPUT_RATE_3		0x02UL
#define COMPASS_DATA_OUTPUT_RATE_7_5	0x03UL
#define COMPASS_DATA_OUTPUT_RATE_15		0x04UL
#define COMPASS_DATA_OUTPUT_RATE_30		0x05UL
#define COMPASS_DATA_OUTPUT_RATE_75		0x06UL

#define COMPASS_GAIN_1370	0x00UL
#define COMPASS_GAIN_1090	0x01UL
#define COMPASS_GAIN_820	0x02UL
#define COMPASS_GAIN_660	0x03UL
#define COMPASS_GAIN_440	0x04UL
#define COMPASS_GAIN_390	0x05UL
#define COMPASS_GAIN_330	0x06UL
#define COMPASS_GAIN_230	0x07UL

#define COMPASS_I2C_STANDARD	0x00UL
#define COMPASS_I2C_FAST		0x01UL

#define COMPASS_ID_A		0x48UL
#define COMPASS_ID_B		0x34UL
#define COMPASS_ID_C		0x33UL

#define COMPASS_CONFIG_1	(0x00UL|(COMPASS_AVERAGE_SAMPLES<<5)|(COMPASS_DATA_OUTPUT_RATE<<2))
#define COMPASS_CONFIG_2	(0x00UL|(COMPASS_GAIN<<5))
#define COMPASS_MODE		(0x00UL|(COMPASS_I2C<<7))

#define COMPASS_DECLINATION_OFFSET		4.78

#define COMPASS_X_OFFSET_1370	-230
#define COMPASS_X_OFFSET_1090	-186
#define COMPASS_X_OFFSET_820	-148
#define COMPASS_X_OFFSET_660	-114
#define COMPASS_X_OFFSET_440	-75
#define COMPASS_X_OFFSET_390	-70
#define COMPASS_X_OFFSET_330	-54
#define COMPASS_X_OFFSET_230	-39

#define COMPASS_Y_OFFSET_1370	254
#define COMPASS_Y_OFFSET_1090	165
#define COMPASS_Y_OFFSET_820	134
#define COMPASS_Y_OFFSET_660	109
#define COMPASS_Y_OFFSET_440	68
#define COMPASS_Y_OFFSET_390	58
#define COMPASS_Y_OFFSET_330	55
#define COMPASS_Y_OFFSET_230	33

#if COMPASS_GAIN == COMPASS_GAIN_1370
#define COMPASS_X_OFFSET	COMPASS_X_OFFSET_1370
#define COMPASS_Y_OFFSET	COMPASS_Y_OFFSET_1370

#elif COMPASS_GAIN == COMPASS_GAIN_1090
#define COMPASS_X_OFFSET	COMPASS_X_OFFSET_1090
#define COMPASS_Y_OFFSET	COMPASS_Y_OFFSET_1090

#elif COMPASS_GAIN == COMPASS_GAIN_820
#define COMPASS_X_OFFSET	COMPASS_X_OFFSET_820
#define COMPASS_Y_OFFSET	COMPASS_Y_OFFSET_820

#elif COMPASS_GAIN == COMPASS_GAIN_660
#define COMPASS_X_OFFSET	COMPASS_X_OFFSET_660
#define COMPASS_Y_OFFSET	COMPASS_Y_OFFSET_660

#elif COMPASS_GAIN == COMPASS_GAIN_440
#define COMPASS_X_OFFSET	COMPASS_X_OFFSET_440
#define COMPASS_Y_OFFSET	COMPASS_Y_OFFSET_440

#elif COMPASS_GAIN == COMPASS_GAIN_390
#define COMPASS_X_OFFSET	COMPASS_X_OFFSET_390
#define COMPASS_Y_OFFSET	COMPASS_Y_OFFSET_390

#elif COMPASS_GAIN == COMPASS_GAIN_330
#define COMPASS_X_OFFSET	COMPASS_X_OFFSET_330
#define COMPASS_Y_OFFSET	COMPASS_Y_OFFSET_330

#elif COMPASS_GAIN == COMPASS_GAIN_230
#define COMPASS_X_OFFSET	COMPASS_X_OFFSET_230
#define COMPASS_Y_OFFSET	COMPASS_Y_OFFSET_230

#endif

/*End of exported constants*/

/*Prototypes*/

/*This function is to initiate the compass*/
void HAL_Copmass_Init(I2C_HandleTypeDef *I2Cx);

/*This function is to read the value from the compass*/
void HAL_Compass_Get(I2C_HandleTypeDef *I2Cx, Compass_TypeDef *DataStruct);

/*This function is to test the compass*/
HAL_StatusTypeDef HAL_Compass_Test(I2C_HandleTypeDef *I2Cx);

/*End of prototypes*/


#endif
