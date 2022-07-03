#ifndef __IMU_H__
#define __IMU_H__

/*Includes*/

#include "imu_reg.h"
#include "imu_config.h"

/*End of includes*/

/*Exported types*/

typedef struct
{
    int16_t Accel_X_RAW;	/*16-bit variable containing the ADC number representing the acceleration in x-axies*/
    int16_t Accel_Y_RAW;	/*16-bit variable containing the ADC number representing the acceleration in y-axies*/
    int16_t Accel_Z_RAW;	/*16-bit variable containing the ADC number representing the acceleration in z-axies*/
    double Ax;				/*Acceleration (a) in x-axies measured in meter per seconds square (m/(s^2))*/
    double Ay;				/*Acceleration (a) in y-axies measured in meter per seconds square (m/(s^2))*/
    double Az;				/*Acceleration (a) in z-axies measured in meter per seconds square (m/(s^2))*/

    int16_t Gyro_X_RAW;		/*16-bit variable containing the ADC number representing the angular velocity in x-axies*/
    int16_t Gyro_Y_RAW;		/*16-bit variable containing the ADC number representing the angular velocity in y-axies*/
    int16_t Gyro_Z_RAW;		/*16-bit variable containing the ADC number representing the angular velocity in z-axies*/
    double Gx;				/*Angular velocity (omega(w)) in x-axies measured in degrees per second (dps)*/
    double Gy;				/*Angular velocity (omega(w)) in y-axies measured in degrees per second (dps)*/
    double Gz;				/*Angular velocity (omega(w)) in z-axies measured in degrees per second (dps)*/

}IMU_TypeDef;

/*End of exported types*/

/*Private defines*/

extern 	IMU_TypeDef		himu;

/*End of private defines*/

/*Exported constants*/

#define IMU_GYRO_SELF_TEST_ON		0b11100000
#define IMU_GYRO_SELF_TEST_OFF		0b00000000
#define IMU_ACCEL_SELF_TEST_ON		0b11110000
#define IMU_ACCEL_SELF_TEST_OFF		0b00010000

#define	IMU_POWER_ON				0b00001000
#define	IMU_SAMPLE_RATE_PRESCALER	0x00UL

#define	IMU_SIGNAL_PATH_RESET_ALL	0b00000111
#define IMU_DEVICE_RESET			0b10001000

#define IMU_GYRO_SCALE_250			0x00UL
#define IMU_GYRO_SCALE_500			0x01UL
#define IMU_GYRO_SCALE_1000			0x02UL
#define IMU_GYRO_SCALE_2000			0x03UL

#define	IMU_ACCEL_SCALE_2G			0x00UL
#define IMU_ACCEL_SCALE_4G			0x01UL
#define IMU_ACCEL_SCALE_8G			0x02UL
#define IMU_ACCEL_SCALE_16G			0x03UL

#define IMU_AD0_RESET		0x00UL
#define IMU_AD0_SET			0x01UL

#if (IMU_AD0_STATE == IMU_AD0_RESET)
#define	IMU_ADDR			0xD0

#elif	(IMU_AD0_STATE == IMU_AD0_SET)
#define	IMU_ADDR			0xD2

#else
#error "IMU_AD0_STATE is not defined correctly."
#error "Please refer to the file imu_config.h and edit the configuration."

#endif

/*End of exported constants*/


/*Prototypes*/

/*This function is to initiate the IMU*/
void HAL_IMU_Init(I2C_HandleTypeDef *I2Cx);

/*This function is to read the acceleration*/
void HAL_IMU_Read_Accel(I2C_HandleTypeDef *I2Cx, IMU_TypeDef *DataStruct);

/*This function is to read the angular velocity*/
void HAL_IMU_Read_Gyro(I2C_HandleTypeDef *I2Cx, IMU_TypeDef *DataStruct);

/*This function is to read both acceleration and angular velocity*/
void HAL_IMU_Read(I2C_HandleTypeDef *I2Cx, IMU_TypeDef *DataStruct);

/*This function is to test the sensors of the IMU*/
HAL_StatusTypeDef HAL_IMU_SensorsTest(I2C_HandleTypeDef *I2Cx);

/*This function is to test the IMU*/
HAL_StatusTypeDef HAL_IMU_Test(I2C_HandleTypeDef *I2Cx);

/*This function is to reset the IMU*/
void HAL_IMU_Reset(I2C_HandleTypeDef *I2Cx);

/*End of prototypes*/


#endif
