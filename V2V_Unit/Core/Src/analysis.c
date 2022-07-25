#include "analysis.h"
#include "imu.h"
#include "NMEA.h"
#include "irencoder.h"
#include "compass.h"
// General Arrays Declaration
char bufferRX[RX_SIZE]; //= "30.123456,31.123455,30.340000,+2.110000,-3.110000,30.150000,15.231000,20.514023,20.1,!!!!!!!!!!!!!!!!!!!!";
char bufferTX[TX_SIZE];
char tempArray[20] = "00000000000000000000";

// General Indexes Declaration
uint16_t indexBufferRX = 0;
uint16_t indexBufferTX = 0;
int globalIndex;
uint16_t indexTempArray = 0;

// General Variables Declaration
uint8_t checkInt = 1;
float splitValue;

// Define cars structure
data_t neighbor;
data_t car;

// Analysis variables
uint8_t t = 0;
float xDiff = 0;
float yDiff = 0;

//GPS data
extern gps_t gpsData;
extern uint16_t FWDistance;
extern uint16_t BWDistance;

void resetBuffersIndexes(void)
{
	indexBufferTX = 0;
	indexBufferRX = 0;
}

float splitData(void)
{
	indexTempArray = 0;
	while(bufferRX[indexBufferRX] != ',' && bufferRX[indexBufferRX] != '!')
	{
		tempArray[indexTempArray] = bufferRX[indexBufferRX];
		if(bufferRX[indexBufferRX] == '.') checkInt = 0;
		indexTempArray++;
		indexBufferRX++;
	}
	if(checkInt == 1) tempArray[indexTempArray] = '.';
	checkInt = 1;
	splitValue = atof(tempArray);
	for(globalIndex = 0; globalIndex < 20; globalIndex++)
		tempArray[globalIndex] = '0';
	return splitValue;
}

void getNeighborData(void)
{
	// Get longitude
	neighbor.longitude = splitData();

	// Get latitude
	indexBufferRX++;
	neighbor.latitude = splitData();

	// Get speed
	indexBufferRX++;
	neighbor.speed = splitData();

	// Get Ax
	indexBufferRX++;
	neighbor.Ax = splitData();

	// Get Ay
	indexBufferRX++;
	neighbor.Ay = splitData();

	// Get Angle
	indexBufferRX++;
	neighbor.angle = splitData();

	// Get Vx
	indexBufferRX++;
	neighbor.Vx = splitData();

	// Get Vy
	indexBufferRX++;
	neighbor.Vy = splitData();

}

// ToDo: Linking these variables with sensors
void getCarData(void)
{
	car.longitude = gpsData.longitude;
	car.latitude  = gpsData.latitude;
	//car.angle     = (float) hcompass.Angle_x;
	car.speed     = (float) hirencoder.RPM;
	car.Ax        = (float) himu.Ax;
	car.Ay        = (float) himu.Ay;
	//car.Vx        =         car.speed * (sin((car.angle *PI)/180));
	//car.Vy        =         car.speed * (cos((car.angle *PI)/180));
}

float getAbsolute(float val)
{
	if(val < 0) val *= -1;
	return val;
}

void sendWarning(void)
{
	bufferTX[indexBufferTX++] = 'M';
	bufferTX[indexBufferTX++] = ':';
	bufferTX[indexBufferTX++] = 'W';
	bufferTX[indexBufferTX++] = 'a';
	bufferTX[indexBufferTX++] = 'r';
	bufferTX[indexBufferTX++] = 'n';
	bufferTX[indexBufferTX++] = 'i';
	bufferTX[indexBufferTX++] = 'n';
	bufferTX[indexBufferTX++] = 'g';
	bufferTX[indexBufferTX++] = '?';
}

void analysis(void)
{


#if 0
	for(t = 0; t < 3; t++)
	{
		// Car analysis
		car.dx = car.Vx * t + .5 * car.Ax * t * t;
		car.dy = car.Vy * t + .5 * car.Ay * t * t;

		// Neighbor analysis
		neighbor.dx = neighbor.Vx * t + .5 * neighbor.Ax * t * t;
		neighbor.dy = neighbor.Vy * t + .5 * neighbor.Ay * t * t;

		// New Cartesian points for this car
		car.x2 = car.dx + car.longitude;
		car.y2 = car.dy + car.latitude;

		// New Cartesian points for neighbor car
		neighbor.x2 = neighbor.dx + neighbor.longitude;
		neighbor.y2 = neighbor.dy + neighbor.latitude;

		xDiff = neighbor.x2 - car.x2;
		yDiff = neighbor.y2 - car.y2;
		if((getAbsolute(xDiff) <= RADIUS_MIN_RANGE)  || (getAbsolute(yDiff) <= RADIUS_MIN_RANGE))
		{
			sendWarning();
			break;
		}

	}
#endif
}

void mergeData(float value)
{

	gcvt(value, TEMP_ARR_SIZE, tempArray);

	for(indexTempArray = 0; indexTempArray < MAX_FLOAT_DIGITS; indexTempArray++)
	{
		if(tempArray[indexTempArray] == '\0') break;
		bufferTX[indexBufferTX] = tempArray[indexTempArray];
		indexBufferTX++;
	}
	bufferTX[indexBufferTX++] = ',';


	for(globalIndex = 0; globalIndex < TEMP_ARR_SIZE; globalIndex++)
		tempArray[globalIndex] = '0';
}

void generateTransmitBuffer(void)
{
	bufferTX[indexBufferTX++] = 'D';
	bufferTX[indexBufferTX++] = ':';

	mergeData(car.longitude);
	mergeData(car.latitude);
	mergeData(car.speed);
	mergeData(car.Ax);
	mergeData(car.Ay);
	//mergeData(car.angle);
	//mergeData(car.Vx);
	//mergeData(car.Vy);
	mergeData(FWDistance);
	mergeData(BWDistance);
	bufferTX[indexBufferTX++] = '?';

	for(; indexBufferTX < TX_SIZE; indexBufferTX++)
	{
		bufferTX[indexBufferTX] = '!';
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{


}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{


}
