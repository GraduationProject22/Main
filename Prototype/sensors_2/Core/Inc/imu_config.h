#ifndef	__IMU_CONFIG_H__
#define	__IMU_CONFIG_H__

/*	Defining the state of the pin AD0 of the IMU.
	IMU_AD0_STATE can be either of the following states:
	1) if AD0 is tied to ground,	IMU_AD0_STATE ----> IMU_AD0_RESET
	2) if AD0 is tied to VCC,		IMU_AD0_STATE ----> IMU_AD0_SET
*/
#define	IMU_AD0_STATE					IMU_AD0_SET

/*	Defining the scale of the gyroscope.
	The gyroscope has 4 scales.
	IMU_GYRO_SCALE can be one of the follwoing values:
	1)For scale from -250 to +250 dps,		IMU_GYRO_SCALE ---> IMU_GYRO_SCALE_250
	2)For scale from -500 to +500 dps,		IMU_GYRO_SCALE ---> IMU_GYRO_SCALE_500
	3)For scale from -1000 to +1000 dps,	IMU_GYRO_SCALE ---> IMU_GYRO_SCALE_1000
	4)For scale from -2000 to +2000 dps,	IMU_GYRO_SCALE ---> IMU_GYRO_SCALE_2000
*/
#define IMU_GYRO_SCALE					IMU_GYRO_SCALE_1000

/*	Defining the scale of the accelerometer.
	The accelerometer has 4 scales.
	IMU_ACCEL_SCALE can be one of the following values:
	1)For scale from -2g to +2g,			IMU_ACCEL_SCALE ---> IMU_ACCEL_SCALE_2G
	2)For scale from -4g to +4g,			IMU_ACCEL_SCALE ---> IMU_ACCEL_SCALE_4G
	3)For scale from -8g to +8g,			IMU_ACCEL_SCALE ---> IMU_ACCEL_SCALE_8G
	4)For scale from -16g to +16g,			IMU_ACCEL_SCALE ---> IMU_ACCEL_SCALE_16G
*/
#define IMU_ACCEL_SCALE					IMU_ACCEL_SCALE_4G

/*	Defining the digital low pass filter(DLPF) order.
	DLPF is set for both accelerometer and gyroscope.
	The next table specifies the characteristics of each order of the DLPF
	-------------------------------------------------------------------------------------------------------------
	|			| Accelerometer (Output Rate = 1kHz)|						Gyroscope							|
	|DLPF Order	|	Bandwidth(Hz)	|	Delay(ms)	|	Bandwidth(Hz)	|	Delay(ms)	|	Output Rate(kHz)	|	
	|-----------|-------------------|---------------|-------------------|---------------|-----------------------|
	|	0		|		260			|		0		|		256			|		0.98	|			8			|
	|-----------|-------------------|---------------|-------------------|---------------|-----------------------|
	|	1		|		184			|		2.0		|		188			|		1.9		|			1			|
	|-----------|-------------------|---------------|-------------------|---------------|-----------------------|
	|	2		|		94			|		3.0		|		98			|		2.8		|			1			|
	|-----------|-------------------|---------------|-------------------|---------------|-----------------------|
	|	3		|		44			|		4.9		|		42			|		4.8		|			1			|
	|-----------|-------------------|---------------|-------------------|---------------|-----------------------|
	|	4		|		21			|		8.5		|		20			|		8.3		|			1			|
	|-----------|-------------------|---------------|-------------------|---------------|-----------------------|
	|	5		|		10			|		13.8	|		10			|		13.4	|			1			|
	|-----------|-------------------|---------------|-------------------|---------------|-----------------------|
	|	6		|		5			|		19.0	|		5			|		18.6	|			1			|
	|-----------|-------------------|---------------|-------------------|---------------|-----------------------|
	|	7		|		---			|		---		|		---			|		---		|			8			|
	-------------------------------------------------------------------------------------------------------------
	IMU_DLPF_ORDER is 3-bit number that can take values from 0x00 to 0x07 
*/
#define	IMU_DLPF_ORDER					0x05UL

/*	Defining the prescaler of the sample rate.
	The prescaler is followed by the next equation
				Sample Rate = (Gyroscope Output Rate)/(Prescaler + 1)
	Gyroscope output rate is 8kHz if the DLPF is disabled, and it is 1kHz if the DLPF is enabled.
	The output rate of accelerometer is always 1kHz.
	IMU_SAMPLE_RATE_PRESCALER is 8-bit number that can take values from 0x00 to 0xFF
*/
#define	IMU_SAMPLE_RATE_PRESCALER		0x00UL
	
#endif
