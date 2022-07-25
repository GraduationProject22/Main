/*Includes*/

#include "main.h"
#include <math.h>
#include "imu.h"

/*End of includes*/

/*Private defines*/

IMU_TypeDef 	himu;
const uint16_t 	i2c_timeout = 100;

/*End of private defines*/

/*Functions*/

/*This function is to initiate the IMU*/
void HAL_IMU_Init(I2C_HandleTypeDef *I2Cx)
{
	uint8_t data_write;


	/*Testing the IMU*/
	if (HAL_IMU_Test(I2Cx) != HAL_OK)
	{
		Error_Handler();
	}

	/*Powering on the IMU*/
	data_write = IMU_POWER_ON|0x01;
	HAL_I2C_Mem_Write(I2Cx, IMU_ADDR, PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &data_write, 1, i2c_timeout);

	/*Defining the digital LPF order (3-bit number)*/
	data_write = (IMU_DLPF_ORDER & 0b00000111);
	HAL_I2C_Mem_Write(I2Cx, IMU_ADDR, CONFIG, I2C_MEMADD_SIZE_8BIT, &data_write, 1, i2c_timeout);

	/*Defining the scale of the gyroscope*/
	data_write = (IMU_GYRO_SCALE << 3);
	HAL_I2C_Mem_Write(I2Cx, IMU_ADDR, GYRO_CONFIG, I2C_MEMADD_SIZE_8BIT, &data_write, 1, i2c_timeout);

	/*Defining the scale of the accelerometer*/
	data_write = (IMU_ACCEL_SCALE << 3);
	HAL_I2C_Mem_Write(I2Cx, IMU_ADDR, ACCEL_CONFIG, I2C_MEMADD_SIZE_8BIT, &data_write, 1, i2c_timeout);
}

/*This function is to read the acceleration*/
void HAL_IMU_Read_Accel(I2C_HandleTypeDef *I2Cx, IMU_TypeDef *DataStruct)
{
    uint8_t Rec_Data[6];
	uint8_t scale = 1;

    /* Read 6 bytes of data starting from ACCEL_XOUT_H register */
    if(HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT, Rec_Data, 6, i2c_timeout) != HAL_OK)
	{
    	/*Reinitiate I2C*/
    	HAL_I2C_Init(I2Cx);
    	/*Reinitiate the IMU*/
//    	HAL_IMU_Init(I2Cx);
	}
	else
	{
		/*Assigning the values*/
		DataStruct->Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
		DataStruct->Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data[3]);
		DataStruct->Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data[5]);

		/*Converting the raw values into meter per seconds square (m/(s^2))*/
		
		for (uint8_t i = 0; i < IMU_ACCEL_SCALE; i++)
		{
			scale *= 2;
		}
	
		DataStruct->Ax = ((DataStruct->Accel_X_RAW * scale * 2) / 32768.0)*9.8;
		DataStruct->Ay = ((DataStruct->Accel_Y_RAW * scale * 2) / 32768.0)*9.8;
		DataStruct->Az = ((DataStruct->Accel_Z_RAW * scale * 2) / 32768.0)*9.8;
	}

}

/*This function is to read the angular velocity*/
void HAL_IMU_Read_Gyro(I2C_HandleTypeDef *I2Cx, IMU_TypeDef *DataStruct)
{
    uint8_t Rec_Data[6];
	uint8_t scale = 1;

    /* Read 6 bytes of data starting from GYRO_XOUT_H register */
    if(HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, GYRO_XOUT_H, I2C_MEMADD_SIZE_8BIT, Rec_Data, 6, i2c_timeout) != HAL_OK)
	{
    	/*Reinitiate I2C*/
    	HAL_I2C_Init(I2Cx);
    	/*Reinitiate the IMU*/
//    	HAL_IMU_Init(I2Cx);
	}
	else
	{
		/*Assigning the values*/
		DataStruct->Gyro_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
		DataStruct->Gyro_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data[3]);
		DataStruct->Gyro_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data[5]);

		/*Converting the raw values into degrees per seconds (dps)*/
    
		for (uint8_t i = 0; i < IMU_GYRO_SCALE; i++)
		{
			scale *= 2;
		}
	
		DataStruct->Gx = (DataStruct->Gyro_X_RAW * scale * 250) / 32768.0;
		DataStruct->Gy = (DataStruct->Gyro_Y_RAW * scale * 250) / 32768.0;
		DataStruct->Gz = (DataStruct->Gyro_Z_RAW * scale * 250) / 32768.0;
	}
	
}

/*This function is to read both acceleration and angular velocity*/
void HAL_IMU_Read(I2C_HandleTypeDef *I2Cx, IMU_TypeDef *DataStruct)
{
    HAL_IMU_Read_Accel(I2Cx, DataStruct);
	//HAL_IMU_Read_Gyro(I2Cx, DataStruct);
}

/*This function is to test the sensors of the IMU*/
HAL_StatusTypeDef HAL_IMU_SensorsTest(I2C_HandleTypeDef *I2Cx)
{
	/*Dummy variable for data*/
	uint8_t	data_write;
	uint8_t data_read;
	/*Accelerometer data*/
	uint16_t accelx_data_st_dis = 0;
	uint16_t accelx_data_st_en = 0;
	int16_t accelx_str = 0;
	uint8_t accelx_test = 0;
	int16_t accelx_ft = 0;
	int16_t accelx_ratio = 0;
	uint16_t accely_data_st_dis = 0;
	uint16_t accely_data_st_en = 0;
	int16_t accely_str = 0;
	uint8_t accely_test = 0;
	int16_t accely_ft = 0;
	int16_t accely_ratio = 0;
	uint16_t accelz_data_st_dis = 0;
	uint16_t accelz_data_st_en = 0;
	int16_t accelz_str = 0;
	uint8_t accelz_test = 0;
	int16_t accelz_ft = 0;
	int16_t accelz_ratio = 0;
	/*Gyroscope data*/
	uint16_t gyrox_data_st_dis = 0;
	uint16_t gyrox_data_st_en = 0;
	int16_t gyrox_str = 0;
	uint8_t gyrox_test = 0;
	int16_t gyrox_ft = 0;
	int16_t gyrox_ratio = 0;
	uint16_t gyroy_data_st_dis = 0;
	uint16_t gyroy_data_st_en = 0;
	int16_t gyroy_str = 0;
	uint8_t gyroy_test = 0;
	int16_t gyroy_ft = 0;
	int16_t gyroy_ratio = 0;
	uint16_t gyroz_data_st_dis = 0;
	uint16_t gyroz_data_st_en = 0;
	int16_t gyroz_str = 0;
	uint8_t gyroz_test = 0;
	int16_t gyroz_ft = 0;
	int16_t gyroz_ratio = 0;

	/*Testing the IMU*/
	if (HAL_IMU_Test(I2Cx) != HAL_OK)
	{
		return HAL_ERROR;
	}
	
	/*Powering on the IMU*/
	data_write = IMU_POWER_ON;
	HAL_I2C_Mem_Write(I2Cx, IMU_ADDR, PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT,&data_write, 1, i2c_timeout);
	
	/*Working on 5 Hz bandwidth*/
	data_write = 0x06;
	HAL_I2C_Mem_Write(I2Cx, IMU_ADDR, CONFIG, I2C_MEMADD_SIZE_8BIT, &data_write, 1, i2c_timeout);

	/*Start the self test for acceleration*/
	
	/*First, reading the data when self test is disabled and the range is from -8g to +8g*/
	/*Setting the range*/
	data_write = IMU_ACCEL_SELF_TEST_OFF;
	HAL_I2C_Mem_Write(I2Cx, IMU_ADDR, ACCEL_CONFIG, I2C_MEMADD_SIZE_8BIT, &data_write, 1, i2c_timeout);
	/*Reading the data*/
	/*x-axies*/
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, ACCEL_XOUT_L, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	accelx_data_st_dis |= data_read;
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	accelx_data_st_dis |= (uint16_t)(data_read << 8);
	/*y-axies*/
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, ACCEL_YOUT_L, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	accely_data_st_dis |= data_read;
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, ACCEL_YOUT_H, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	accely_data_st_dis |= (uint16_t)(data_read << 8);
	/*z-axies*/
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, ACCEL_ZOUT_L, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	accelz_data_st_dis |= data_read;
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, ACCEL_ZOUT_H, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	accelz_data_st_dis |= (uint16_t)(data_read << 8);
	
	/*Second, reading the data when the self test is enabled and the range is from -8d to +8g*/
	/*Setting the range*/
	data_write = IMU_ACCEL_SELF_TEST_ON;
	HAL_I2C_Mem_Write(I2Cx, IMU_ADDR, ACCEL_CONFIG, I2C_MEMADD_SIZE_8BIT, &data_write, 1, i2c_timeout);
	/*Reading the data*/
	/*x-axies*/
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, ACCEL_XOUT_L, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	accelx_data_st_en |= data_read;
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	accelx_data_st_en |= (uint16_t)(data_read << 8);
	/*y-axies*/
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, ACCEL_YOUT_L, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	accely_data_st_en |= data_read;
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, ACCEL_YOUT_H, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	accely_data_st_en |= (uint16_t)(data_read << 8);
	/*z-axies*/
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, ACCEL_ZOUT_L, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	accelz_data_st_en |= data_read;
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, ACCEL_ZOUT_H, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	accelz_data_st_en |= (uint16_t)(data_read << 8);
	
	/*Third, calculating the self test response*/
	accelx_str = accelx_data_st_en - accelx_data_st_dis;
	accely_str = accely_data_st_en - accely_data_st_dis;
	accelz_str = accelz_data_st_en - accelz_data_st_dis;
	
	/*Fourth, calculating the factory trim value from the formula from the datasheet*/
	/*Reading the accelerometer self test registers*/
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, SELF_TEST_X, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	accelx_test |= ((data_read & 0b11100000) >> 3);
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, SELF_TEST_Y, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	accely_test |= ((data_read & 0b11100000) >> 3);
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, SELF_TEST_Z, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	accelz_test |= ((data_read & 0b11100000) >> 3);
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, SELF_TEST_A, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	accelx_test |= ((data_read & 0b00110000) >> 4);
	accely_test |= ((data_read & 0b00001100) >> 2);
	accelz_test |= ((data_read & 0b00000011) >> 0);
	/*Calculating the FT*/
	if (accelx_test != 0)
	{
		accelx_ft = (int16_t)(4096 * 0.34 * pow(0.92/0.34,((double)(accelx_test-1))/30));
	}
	if (accely_test != 0)
	{
		accely_ft = (int16_t)(4096 * 0.34 * pow(0.92/0.34,((double)(accely_test-1))/30));
	}
	if (accelz_test != 0)
	{
		accelz_ft = (int16_t)(4096 * 0.34 * pow(0.92/0.34,((double)(accelz_test-1))/30));
	}
	
	/*Fifth, Checking the difference between STR and FT if it is in range from -14% to 14% as indicated in datasheet*/
	accelx_ratio = (accelx_str - accelx_ft)/accelx_ft;
	if ((accelx_ratio > 14)|(accelx_ratio < -14))
	{
		return HAL_ERROR;
	}
	accely_ratio = (accely_str - accely_ft)/accely_ft;
	if ((accely_ratio > 14)|(accely_ratio < -14))
	{
		return HAL_ERROR;
	}
	accelz_ratio = (accelz_str - accelz_ft)/accelz_ft;
	if ((accelz_ratio > 14)|(accelz_ratio < -14))
	{
		return HAL_ERROR;
	}
	
	/*Start the self test for gyroscope*/
	
	/*First, reading the data when self test is disabled and the range is from -250dps to +250dps*/
	/*Setting the range*/
	data_write = IMU_GYRO_SELF_TEST_OFF;
	HAL_I2C_Mem_Write(I2Cx, IMU_ADDR, GYRO_CONFIG, I2C_MEMADD_SIZE_8BIT, &data_write, 1, i2c_timeout);
	/*Reading the data*/
	/*x-axies*/
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, GYRO_XOUT_L, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	gyrox_data_st_dis |= data_read;
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, GYRO_XOUT_H, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	gyrox_data_st_dis |= (uint16_t)(data_read << 8);
	/*y-axies*/
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, GYRO_YOUT_L, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	gyroy_data_st_dis |= data_read;
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, GYRO_YOUT_H, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	gyroy_data_st_dis |= (uint16_t)(data_read << 8);
	/*z-axies*/
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, GYRO_ZOUT_L, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	gyroz_data_st_dis |= data_read;
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, GYRO_ZOUT_H, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	gyroz_data_st_dis |= (uint16_t)(data_read << 8);
	
	/*Second, reading the data when the self test is enabled and the range is from -250dps to +250dps*/
	/*Setting the range*/
	data_write = IMU_GYRO_SELF_TEST_ON;
	HAL_I2C_Mem_Write(I2Cx, IMU_ADDR, GYRO_CONFIG, I2C_MEMADD_SIZE_8BIT, &data_write, 1, i2c_timeout);
	/*Reading the data*/
	/*x-axies*/
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, GYRO_XOUT_L, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	gyrox_data_st_en |= data_read;
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, GYRO_XOUT_H, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	gyrox_data_st_en |= (uint16_t)(data_read << 8);
	/*y-axies*/
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, GYRO_YOUT_L, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	gyroy_data_st_en |= data_read;
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, GYRO_YOUT_H, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	gyroy_data_st_en |= (uint16_t)(data_read << 8);
	/*z-axies*/
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, GYRO_ZOUT_L, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	gyroz_data_st_en |= data_read;
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, GYRO_ZOUT_H, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	gyroz_data_st_en |= (uint16_t)(data_read << 8);
	
	/*Third, calculating the self test response*/
	gyrox_str = gyrox_data_st_en - gyrox_data_st_dis;
	gyroy_str = gyroy_data_st_en - gyroy_data_st_dis;
	gyroz_str = gyroz_data_st_en - gyroz_data_st_dis;
	
	/*Fourth, calculating the factory trim value from the formula from the datasheet*/
	/*Reading the accelerometer self test registers*/
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, SELF_TEST_X, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	gyrox_test |= (data_read & 0b00011111);
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, SELF_TEST_Y, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	gyroy_test |= (data_read & 0b00011111);
	HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, SELF_TEST_Z, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
	gyroz_test |= (data_read & 0b00011111);
	/*Calculating the FT*/
	if (gyrox_test != 0)
	{
		gyrox_ft = (int16_t)(25 * 131 * pow(1.046,gyrox_test-1));
	}
	if (gyroy_test != 0)
	{
		gyroy_ft = (int16_t)(-25 * 131 * pow(1.046,gyroy_test-1));
	}
	if (gyroz_test != 0)
	{
		gyroz_ft = (int16_t)(25 * 131 * pow(1.046,gyroz_test-1));
	}
	/*Fifth, Checking the difference between STR and FT if it is in range from -14% to 14% as indicated in datasheet*/
	gyrox_ratio = (gyrox_str - gyrox_ft)/gyrox_ft;
	if ((gyrox_ratio > 14)|(gyrox_ratio < -14))
	{
		return HAL_ERROR;
	}
	gyroy_ratio = (gyroy_str - gyroy_ft)/gyroy_ft;
	if ((gyroy_ratio > 14)|(gyroy_ratio < -14))
	{
		return HAL_ERROR;
	}
	gyroz_ratio = (gyroz_str - gyroz_ft)/gyroz_ft;
	if ((gyroz_ratio > 14)|(gyroz_ratio < -14))
	{
		return HAL_ERROR;
	}

	return HAL_OK;
}

/*This function is to test the IMU*/
HAL_StatusTypeDef HAL_IMU_Test(I2C_HandleTypeDef *I2Cx)
{
	uint8_t data_read;
	uint8_t	count = 0;
	
	while (count < i2c_timeout)
	{
		/*Read the register WHO_AM_I*/
		HAL_I2C_Mem_Read(I2Cx, IMU_ADDR, WHO_AM_I, I2C_MEMADD_SIZE_8BIT, &data_read, 1, i2c_timeout);
		/*The register WHO_AM_I is read only register with the value 0x68*/
		if (data_read != 0x68)
		{
			/*Resetting the IMU*/
			HAL_IMU_Reset(I2Cx);
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

/*This function is to reset the IMU*/
void HAL_IMU_Reset(I2C_HandleTypeDef *I2Cx)
{
	uint8_t	data_write;
	
	/*Resting the registers*/
	data_write = IMU_DEVICE_RESET;
	HAL_I2C_Mem_Write(I2Cx, IMU_ADDR, PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &data_write, 1, i2c_timeout);
	
	/*Reseting the signal paths*/
	data_write = IMU_SIGNAL_PATH_RESET_ALL;
	HAL_I2C_Mem_Write(I2Cx, IMU_ADDR, SIGNAL_PATH_RESET, I2C_MEMADD_SIZE_8BIT, &data_write, 1, i2c_timeout);
}

/*End of functions*/
