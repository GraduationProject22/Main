/*Includes*/

#include "main.h"
#include <stdio.h>
#include <math.h>
#include "compass.h"

/*End of includes*/

/*Private defines*/

Compass_TypeDef 	hcompass;
extern  uint16_t i2c_timeout;

/*End of private defines*/

/*Functions*/

/*This function is to initiate the compass*/
void HAL_Copmass_Init(I2C_HandleTypeDef *I2Cx)
{
	uint8_t data_write;


	/*Testing the Compass*/
	if (HAL_Compass_Test(I2Cx) != HAL_OK)
	{
		Error_Handler();
	}

	/*Setting the average samples and data output rate*/
	data_write = COMPASS_CONFIG_1;
	HAL_I2C_Mem_Write(I2Cx, COMPASS_ADDR, CONFIGURATION_REGISTER_A, I2C_MEMADD_SIZE_8BIT, &data_write, 1, i2c_timeout);

	/*Setting the gain*/
	data_write = COMPASS_CONFIG_2;
	HAL_I2C_Mem_Write(I2Cx, COMPASS_ADDR, CONFIGURATION_REGISTER_B, I2C_MEMADD_SIZE_8BIT, &data_write, 1, i2c_timeout);

	/*Setting the I2C mode*/
	data_write = COMPASS_MODE;
	HAL_I2C_Mem_Write(I2Cx, COMPASS_ADDR, MODE_REGISTER, I2C_MEMADD_SIZE_8BIT, &data_write, 1, i2c_timeout);

}

/*This function is to read the value from the compass*/
void HAL_Compass_Get(I2C_HandleTypeDef *I2Cx, Compass_TypeDef *DataStruct)
{
	uint8_t Rec_Data[6] = {'\0'};

	/*Checking if the communication is successful*/
	if (HAL_I2C_Mem_Read(I2Cx, COMPASS_ADDR, DATA_OUTPUT_X_MSB_REGISTER, I2C_MEMADD_SIZE_8BIT, Rec_Data, 6, i2c_timeout) != HAL_OK)
	{
		/*Reinitiate I2C*/
		HAL_I2C_Init(I2Cx);
	}
	else
	{

		/*Saving the raw values*/
		DataStruct->X_RAW = (int16_t)(Rec_Data[0]<<8 | Rec_Data[1]);
		DataStruct->Y_RAW = (int16_t)(Rec_Data[4]<<8 | Rec_Data[5]);

		/*Adjusting the offset error*/
		DataStruct->X_RAW_CAL = DataStruct->X_RAW + COMPASS_X_OFFSET;
		DataStruct->Y_RAW_CAL = DataStruct->Y_RAW + COMPASS_Y_OFFSET;

		/*Calculating the angle*/
		/*Checking if y axies is pointing to north or south*/
		if (DataStruct->X_RAW_CAL == 0)
		{
			/*Checking if y axies is pointing to north*/
			if (DataStruct->Y_RAW_CAL > 0)
			{
				DataStruct->Angle_x = 90.0;
				DataStruct->Angle_y = 0.0;
			}
			/*Checking if y axies is pointing to south*/
			else
			{
				DataStruct->Angle_x = 270.0;
				DataStruct->Angle_y = 180.0;
			}
		}
		/*Checking if x axies is pointing to north or south*/
		else if (DataStruct->Y_RAW_CAL == 0)
		{
			/*Checking if x axies is pointing to north*/
			if (DataStruct->X_RAW_CAL > 0)
			{
				DataStruct->Angle_x = 0.0;
				DataStruct->Angle_y = 270.0;
			}
			/*Checking if x axies is pointing to south*/
			else
			{
				DataStruct->Angle_x = 180.0;
				DataStruct->Angle_y = 90.0;
			}
		}
		/*Checking if the axies aren't pointing directly to any of the directions*/
		else
		{
			if ((DataStruct->X_RAW_CAL > 0)&&(DataStruct->Y_RAW_CAL > 0))
			{
				/*Calculating the angle between x axies and the north*/
				DataStruct->Angle_x = atan((double)(DataStruct->Y_RAW_CAL)/(double)(DataStruct->X_RAW_CAL)) * 180.0 / MATH_PI;
				/*Calculating the angle between y axies and the north*/
				DataStruct->Angle_y = 270.0 + DataStruct->Angle_x;
			}
			else if ((DataStruct->X_RAW_CAL > 0)&&(DataStruct->Y_RAW_CAL < 0))
			{
				/*Calculating the angle between x axies and the north*/
				DataStruct->Angle_x = 360.0 + (atan((double)(DataStruct->Y_RAW_CAL)/(double)(DataStruct->X_RAW_CAL)) * 180.0 / MATH_PI);
				/*Calculating the angle between y axies and the north*/
				DataStruct->Angle_y = DataStruct->Angle_x - 90.0;
			}
			else
			{
				/*Calculating the angle between x axies and the north*/
				DataStruct->Angle_x = 180.0 + (atan((double)(DataStruct->Y_RAW_CAL)/(double)(DataStruct->X_RAW_CAL)) * 180.0 / MATH_PI);
				/*Calculating the angle between y axies and the north*/
				DataStruct->Angle_y = DataStruct->Angle_x - 90.0;
			}
		}

		/*Adding the declination offset*/
		DataStruct->Angle_x += COMPASS_DECLINATION_OFFSET;
		DataStruct->Angle_y += COMPASS_DECLINATION_OFFSET;

		/*adjusting the angle*/
		if (DataStruct->Angle_x >= 360.0)
		{
			DataStruct->Angle_x -= 360.0;
		}
		if (DataStruct->Angle_y >= 360.0)
		{
			DataStruct->Angle_y -= 360.0;
		}
	}
}

/*This function is to test the compass*/
HAL_StatusTypeDef HAL_Compass_Test(I2C_HandleTypeDef *I2Cx)
{
	uint8_t data_read[3] = {'\0'};
	uint8_t	count = 0;

	while (count < i2c_timeout)
	{
		/*Read the Identification registers A & B & C*/
		HAL_I2C_Mem_Read(I2Cx, COMPASS_ADDR, IDENTIFICATION_REGISTER_A, I2C_MEMADD_SIZE_8BIT, data_read, 3, i2c_timeout);
		/*The Identification registers are read only register with the value of COMPASS_ID_X*/
		if ((data_read[0] != COMPASS_ID_A)||(data_read[1] != COMPASS_ID_B)||(data_read[2] != COMPASS_ID_C))
		{
			/*Adding count to act as timeout*/
			count++;
		}
		else
		{
			return HAL_OK;
		}

	}

	return HAL_ERROR;
}

/*End of functions*/
