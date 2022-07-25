#ifndef __COMPASS_CONFIG_H__
#define __COMPASS_CONFIG_H__

/*	Defining the samples averaged per measurement output
	COMPASS_AVERAGE_SAMPLES can be defined with one of the following values:
	1) 1 sample averaged	-->		COMPASS_AVERAGE_SAMPLES_1
	2) 2 samples averaged	-->		COMPASS_AVERAGE_SAMPLES_2
	3) 4 samples averaged	-->		COMPASS_AVERAGE_SAMPLES_4
	4) 8 samples averaged	-->		COMPASS_AVERAGE_SAMPLES_8
	*/
#define COMPASS_AVERAGE_SAMPLES		COMPASS_AVERAGE_SAMPLES_8

/*	Defining the data output rate
	COMPASS_DATA_OUTPUT_RATE can be defined with one of the following values:
	1) 0.75 Hz				-->		COMPASS_DATA_OUTPUT_RATE_0_75
	2) 1.5 Hz				-->		COMPASS_DATA_OUTPUT_RATE_1_5
	3) 3 Hz					-->		COMPASS_DATA_OUTPUT_RATE_3
	4) 7.5 Hz				-->		COMPASS_DATA_OUTPUT_RATE_7_5
	5) 15 Hz				-->		COMPASS_DATA_OUTPUT_RATE_15
	6) 30 Hz				-->		COMPASS_DATA_OUTPUT_RATE_30
	7) 75 Hz				-->		COMPASS_DATA_OUTPUT_RATE_75
*/
#define COMPASS_DATA_OUTPUT_RATE	COMPASS_DATA_OUTPUT_RATE_75

/*	Defining the gain value
	The next table defines the recommended range with the gain
	-----------------------------------------------------------------------------
	|	Recommended Range (Ga)	|	Gain(LSB/Ga)	|	Resolution (mGa/LSB)	|	
	|---------------------------|-------------------|---------------------------|
	|		-0.88 to +0.88		|		1370		|			0.73			|
	|---------------------------|-------------------|---------------------------|
	|		-1.3 to +1.3		|		1090		|			0.92			|
	|---------------------------|-------------------|---------------------------|
	|		-1.9 to +1.9		|		820			|			1.22			|
	|---------------------------|-------------------|---------------------------|
	|		-2.5 to +2.5		|		660			|			1.52			|
	|---------------------------|-------------------|---------------------------|
	|		-4.0 to +4.0		|		440			|			2.27			|
	|---------------------------|-------------------|---------------------------|
	|		-4.7 to +4.7		|		390			|			2.56			|
	|---------------------------|-------------------|---------------------------|
	|		-5.6 to +5.6		|		330			|			3.03			|
	|---------------------------|-------------------|---------------------------|
	|		-8.1 to +8.1		|		230			|			4.35			|
	|---------------------------|-------------------|---------------------------|
	
	COMPASS_GAIN can be defined with one of the following values.
	1) 1370 LSB/Ga			--> 	COMPASS_GAIN_1370
	2) 1090 LSB/Ga			-->		COMPASS_GAIN_1090
	3) 820 LSB/Ga			-->		COMPASS_GAIN_820
	4) 660 LSB/Ga			-->		COMPASS_GAIN_660
	5) 440 LSB/Ga			-->		COMPASS_GAIN_440
	6) 390 LSB/Ga			-->		COMPASS_GAIN_390
	7) 330 LSB/Ga			-->		COMPASS_GAIN_330
	8) 230 LSB/Ga			-->		COMPASS_GAIN_230
*/
#define	COMPASS_GAIN				COMPASS_GAIN_390

/*	Defining the mode of the I2C protocol
	COMPASS_I2C should be one of the following values.
	1) standard mode (100 kHz)		--> COMPASS_I2C_STANDARD
	2) fast mode (400 kHz)			-->	COMPASS_I2C_FAST
*/	
#define COMPASS_I2C					COMPASS_I2C_STANDARD

#endif
