
#include "Rasp_Driver.h"
#include "main.h"



extern UART_HandleTypeDef huart1;
extern void MX_USART1_UART_Init(void);

								/*** Variables. ***/
// Warning messages for neighbor cars.
uint8_t reduceSpeedWarningMsg[TX_SIZE] = "Warning! Reduce your speed immediately????????????";
uint8_t holdOnSpeedWarningMsg[TX_SIZE] = "Warning! Keep your pace???????????????????????????";
uint8_t boostSpeedWarningMsg[TX_SIZE]  = "Warning! Boost your speed immediately?????????????";
uint8_t rightSignWarningMsg[TX_SIZE]   = "Warning! Some car in left lane may join yours?????";
uint8_t leftSignWarningMsg[TX_SIZE]    = "Warning! Some car in right lane may join yours????";
uint8_t rightDoorWarningMsg[TX_SIZE]   = "Warning! Some door in left lane may be opened?????";
uint8_t leftDoorWarningMsg[TX_SIZE]    = "Warning! Some door in right lane may be opened????";
uint8_t bagDoorWarningMsg[TX_SIZE]     = "Warning! Bag door in some front car may be opened?";
uint8_t carHoodWarningMsg[TX_SIZE]     = "Warning! Car hood in some behind car may be opened";
uint8_t changeLaneWarningMsg[TX_SIZE]  = "Warning! Don't change your lane right now?????????";
uint8_t redundantSafeMsg[TX_SIZE]      = "                                                  ";

// TX and RX buffer initialization.
uint8_t bufferRX[RX_SIZE];
uint8_t bufferTX[TX_SIZE] = "s=???,a=+??,x=+?,y=+?,r=?,l=?,d=?.................";
uint8_t totalTXBuffer[200];
uint16_t indexoFtotalTXBuffer = 0;
int indexOfRXBuffer = 0;
int indexOfTXBuffer = 0;
int globalIndex = 0;

// Neighbor car Data initialization
uint8_t speed = 0;
int8_t accerlation = 0;
int8_t  xAxis = 0;
int8_t  yAxis = 0;
uint8_t leftSignCheck  = 0;
uint8_t rightSignCheck = 0;
uint8_t doorSignCheck  = 0;
uint8_t positionIndication = 0;

// This car data initialization.
uint8_t carSpeed       = 60;
int8_t carAccerlation = -3;
uint8_t carLeftSign    = 1;
uint8_t carRightSign  = 0;
uint8_t carDoorSign  = BAG_DOOR;
int8_t carXAxis = 0;
int8_t carYAxis = 0;


								/*** Functions Definitions. ***/
uint8_t calculateValue(int indexOfRXBuffer)
{
	long power = 1;
    uint8_t result = 0;
	while(bufferRX[indexOfRXBuffer] != '=' && bufferRX[indexOfRXBuffer] != '+' && bufferRX[indexOfRXBuffer] != '-')
	{
		result += (bufferRX[indexOfRXBuffer] - '0') * power;
		power *= 10;
		indexOfRXBuffer--;
	}
	return result;
}

uint8_t takeAbsoulute(int8_t value)
{
	if(value < 0) return value*-1;
	return value;
}

void convertIntToChars(uint8_t maxDigit, uint8_t value)
{
	long power = 1;
	int index;
	for(index = 1; index < maxDigit; index++) power *= 10;

	for(index = maxDigit; index > 0; index --)
	{
		bufferTX[indexOfTXBuffer] = (value/power) + '0';
		value -= (bufferTX[indexOfTXBuffer] - '0') * power;
		indexOfTXBuffer++;
		power /= 10;
	}
}

void indicatePosition()
{
	if(xAxis > 0  && yAxis > 0) positionIndication = NORTH_EAST;
	if(xAxis > 0  && yAxis < 0) positionIndication = SOUTH_EAST;
	if(xAxis < 0  && yAxis > 0) positionIndication = NORTH_WEST;
	if(xAxis < 0  && yAxis < 0) positionIndication = SOUTH_WEST;
	if(xAxis == 0 && yAxis > 0) positionIndication = NORTH;
	if(xAxis == 0 && yAxis < 0) positionIndication = SOUTH;
	if(xAxis > 0  && yAxis == 0) positionIndication = EAST;
	if(xAxis < 0  && yAxis == 0) positionIndication = WEST;
}

void sendWarning()
{
	if(positionIndication == NORTH_EAST)
	{
		if(doorSignCheck == LEFT_DOOR)
			for(globalIndex = 0; globalIndex < TX_SIZE; globalIndex++)
			{
				totalTXBuffer[indexoFtotalTXBuffer] = leftDoorWarningMsg[globalIndex];
				indexoFtotalTXBuffer++;
			}
		if(leftSignCheck == 1) //HAL_UART_Transmit(&huart1, leftSignWarningMsg, TX_SIZE, 0xFF);
			for(globalIndex = 0; globalIndex < TX_SIZE; globalIndex++)
			{
				totalTXBuffer[indexoFtotalTXBuffer] = leftSignWarningMsg[globalIndex];
				indexoFtotalTXBuffer++;
			}
		// Todo: Calculate the distances and compare.
		if(speed < carSpeed && accerlation < 0) //HAL_UART_Transmit(&huart1, reduceSpeedWarningMsg, TX_SIZE, 0xFF);
			for(globalIndex = 0; globalIndex < TX_SIZE; globalIndex++)
			{
				totalTXBuffer[indexoFtotalTXBuffer] = reduceSpeedWarningMsg[globalIndex];
				indexoFtotalTXBuffer++;
			}
	}
	if(positionIndication == NORTH_WEST)
	{
		if(rightSignCheck == 1) //HAL_UART_Transmit(&huart1, rightSignWarningMsg, TX_SIZE, 0xFF);
			for(globalIndex = 0; globalIndex < TX_SIZE; globalIndex++)
			{
				totalTXBuffer[indexoFtotalTXBuffer] = rightSignWarningMsg[globalIndex];
				indexoFtotalTXBuffer++;
			}
		// Todo: Calculate the distances by second Law Newten and compare.
		if(speed < carSpeed && accerlation < 0) //HAL_UART_Transmit(&huart1, reduceSpeedWarningMsg, TX_SIZE, 0xFF);
			for(globalIndex = 0; globalIndex < TX_SIZE; globalIndex++)
			{
				totalTXBuffer[indexoFtotalTXBuffer] = reduceSpeedWarningMsg[globalIndex];
				indexoFtotalTXBuffer++;
			}
		if(doorSignCheck == RIGHT_DOOR) //HAL_UART_Transmit(&huart1, rightDoorWarningMsg, TX_SIZE, 0xFF);
			for(globalIndex = 0; globalIndex < TX_SIZE; globalIndex++)
			{
				totalTXBuffer[indexoFtotalTXBuffer] = rightDoorWarningMsg[globalIndex];
				indexoFtotalTXBuffer++;
			}
	}
	if(positionIndication == NORTH)
	{
		if(speed < carSpeed && accerlation < 0) //HAL_UART_Transmit(&huart1, reduceSpeedWarningMsg, TX_SIZE, 0xFF);
			for(globalIndex = 0; globalIndex < TX_SIZE; globalIndex++)
			{
				totalTXBuffer[indexoFtotalTXBuffer] = reduceSpeedWarningMsg[globalIndex];
				indexoFtotalTXBuffer++;
			}
		if(doorSignCheck == BAG_DOOR && yAxis == 1) //HAL_UART_Transmit(&huart1, bagDoorWarningMsg, TX_SIZE, 0xFF);
			for(globalIndex = 0; globalIndex < TX_SIZE; globalIndex++)
			{
				totalTXBuffer[indexoFtotalTXBuffer] = bagDoorWarningMsg[globalIndex];
				indexoFtotalTXBuffer++;
			}
	}
	if(positionIndication == SOUTH_WEST)
	{
		if(speed > carSpeed && accerlation > 0 && rightSignCheck == 1) //HAL_UART_Transmit(&huart1, boostSpeedWarningMsg, TX_SIZE, 0xFF);
			for(globalIndex = 0; globalIndex < TX_SIZE; globalIndex++)
			{
				totalTXBuffer[indexoFtotalTXBuffer] = boostSpeedWarningMsg[globalIndex];
				indexoFtotalTXBuffer++;
			}
		if(speed > carSpeed && carLeftSign == 1) //HAL_UART_Transmit(&huart1, changeLaneWarningMsg, TX_SIZE, 0xFF);
			for(globalIndex = 0; globalIndex < TX_SIZE; globalIndex++)
			{
				totalTXBuffer[indexoFtotalTXBuffer] = changeLaneWarningMsg[globalIndex];
				indexoFtotalTXBuffer++;
			}
	}
	if(positionIndication == SOUTH_EAST)
	{
		if(speed > carSpeed && accerlation > 0 && leftSignCheck == 1) //HAL_UART_Transmit(&huart1, boostSpeedWarningMsg, TX_SIZE, 0xFF);
			for(globalIndex = 0; globalIndex < TX_SIZE; globalIndex++)
			{
				totalTXBuffer[indexoFtotalTXBuffer] = boostSpeedWarningMsg[globalIndex];
				indexoFtotalTXBuffer++;
			}
		if(speed > carSpeed && carRightSign == 1) //HAL_UART_Transmit(&huart1, changeLaneWarningMsg, TX_SIZE, 0xFF);
			for(globalIndex = 0; globalIndex < TX_SIZE; globalIndex++)
			{
				totalTXBuffer[indexoFtotalTXBuffer] = changeLaneWarningMsg[globalIndex];
				indexoFtotalTXBuffer++;
			}
	}
	if(positionIndication == SOUTH)
	{
		if(speed > carSpeed && accerlation > 0) //HAL_UART_Transmit(&huart1, boostSpeedWarningMsg, TX_SIZE, 0xFF);
			for(globalIndex = 0; globalIndex < TX_SIZE; globalIndex++)
			{
				totalTXBuffer[indexoFtotalTXBuffer] = boostSpeedWarningMsg[globalIndex];
				indexoFtotalTXBuffer++;
			}
		if(doorSignCheck == CAR_HOOD && yAxis == -1) //HAL_UART_Transmit(&huart1, carHoodWarningMsg, TX_SIZE, 0xFF);
			for(globalIndex = 0; globalIndex < TX_SIZE; globalIndex++)
			{
				totalTXBuffer[indexoFtotalTXBuffer] = carHoodWarningMsg[globalIndex];
				indexoFtotalTXBuffer++;
			}
	}
	if(positionIndication == EAST)
	{
		if(doorSignCheck == LEFT_DOOR) //HAL_UART_Transmit(&huart1, leftDoorWarningMsg, TX_SIZE, 0xFF);
			for(globalIndex = 0; globalIndex < TX_SIZE; globalIndex++)
			{
				totalTXBuffer[indexoFtotalTXBuffer] = leftDoorWarningMsg[globalIndex];
				indexoFtotalTXBuffer++;
			}
		if(leftSignCheck == 1) //HAL_UART_Transmit(&huart1, holdOnSpeedWarningMsg, TX_SIZE, 0xFF);
			for(globalIndex = 0; globalIndex < TX_SIZE; globalIndex++)
			{
				totalTXBuffer[indexoFtotalTXBuffer] = holdOnSpeedWarningMsg[globalIndex];
				indexoFtotalTXBuffer++;
			}
		if(carRightSign == 1 && leftSignCheck == 1) //HAL_UART_Transmit(&huart1, changeLaneWarningMsg, TX_SIZE, 0xFF);
			for(globalIndex = 0; globalIndex < TX_SIZE; globalIndex++)
			{
				totalTXBuffer[indexoFtotalTXBuffer] = changeLaneWarningMsg[globalIndex];
				indexoFtotalTXBuffer++;
			}
	}
	if(positionIndication == WEST)
	{
		if(doorSignCheck == RIGHT_DOOR) //HAL_UART_Transmit(&huart1, rightDoorWarningMsg, TX_SIZE, 0xFF);
			for(globalIndex = 0; globalIndex < TX_SIZE; globalIndex++)
			{
				totalTXBuffer[indexoFtotalTXBuffer] = rightDoorWarningMsg[globalIndex];
				indexoFtotalTXBuffer++;
			}
		if(rightSignCheck == 1) //HAL_UART_Transmit(&huart1, holdOnSpeedWarningMsg, TX_SIZE, 0xFF);
			for(globalIndex = 0; globalIndex < TX_SIZE; globalIndex++)
			{
				totalTXBuffer[indexoFtotalTXBuffer] = holdOnSpeedWarningMsg[globalIndex];
				indexoFtotalTXBuffer++;
			}
		if(carLeftSign == 1 && rightSignCheck == 1) //HAL_UART_Transmit(&huart1, changeLaneWarningMsg, TX_SIZE, 0xFF);
			for(globalIndex = 0; globalIndex < TX_SIZE; globalIndex++)
			{
				totalTXBuffer[indexoFtotalTXBuffer] = changeLaneWarningMsg[globalIndex];
				indexoFtotalTXBuffer++;
			}
	}
	//HAL_UART_Transmit(&huart1, redundantSafeMsg, TX_SIZE, 0xFF);

}

void storeCarVariables()
{
	indexOfTXBuffer = 2;
	// Store Speed.
	convertIntToChars(MAX_SPEED_DIGIT, carSpeed);
	indexOfTXBuffer +=3;

	// Store Accerlation.
	if(carAccerlation < 0) bufferTX[indexOfTXBuffer] = '-';
	else                   bufferTX[indexOfTXBuffer] = '+';
	indexOfTXBuffer++;
	convertIntToChars(MAX_ACCERLATION_DIGIT, takeAbsoulute(carAccerlation));
	indexOfTXBuffer +=3;

	//store XAxis.
	if(xAxis > 0) bufferTX[indexOfTXBuffer] = '-';
	else bufferTX[indexOfTXBuffer] = '+';
	indexOfTXBuffer++;
	bufferTX[indexOfTXBuffer] = takeAbsoulute(xAxis) + '0';
	indexOfTXBuffer +=4;

	//store YAxis.
	if(yAxis > 0) bufferTX[indexOfTXBuffer] = '-';
	else		  bufferTX[indexOfTXBuffer] = '+';
	indexOfTXBuffer++;
	bufferTX[indexOfTXBuffer] = takeAbsoulute(yAxis) + '0';
	indexOfTXBuffer +=4;

	//store Right sign.
	bufferTX[indexOfTXBuffer] = carRightSign + '0';
	indexOfTXBuffer +=4;

	//store Left sign.
	bufferTX[indexOfTXBuffer] = carLeftSign + '0';
	indexOfTXBuffer +=4;

	//store Door sign.
	bufferTX[indexOfTXBuffer] = carDoorSign + '0';
	indexOfTXBuffer +=4;

	for(globalIndex = 0; globalIndex < TX_SIZE; globalIndex++)
	{
		totalTXBuffer[indexoFtotalTXBuffer] = bufferTX[globalIndex];
		indexoFtotalTXBuffer++;
	}
	indexoFtotalTXBuffer = 0;
}

void resetVariables()
{
	speed = 0;
	accerlation = 0;
	xAxis = 0;
	yAxis = 0;
	leftSignCheck  = 0;
	rightSignCheck = 0;
	doorSignCheck  = 0;
	positionIndication = 0;
	carXAxis = 0;
	carYAxis = 0;
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{


}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	indexOfRXBuffer = 0;
	while(indexOfRXBuffer != sizeof(bufferRX))
	{
			if(bufferRX[indexOfRXBuffer] == 's')
			{
				speed = calculateValue(indexOfRXBuffer + MAX_SPEED_DIGIT + 1);
				indexOfRXBuffer += MAX_SPEED_DIGIT + 1;
			}
			if(bufferRX[indexOfRXBuffer] == 'a')
			{
				accerlation = calculateValue(indexOfRXBuffer + MAX_ACCERLATION_DIGIT + 2);
				if(bufferRX[indexOfRXBuffer + 2] == '-') accerlation *= -1;
				indexOfRXBuffer += MAX_ACCERLATION_DIGIT + 2;
			}
			if(bufferRX[indexOfRXBuffer] == 'x')
			{

				xAxis = bufferRX[indexOfRXBuffer + MAX_X_AXIS_DIGIT + 2] - '0';
				if(bufferRX[indexOfRXBuffer + 2] == '-') xAxis *= -1;
				indexOfRXBuffer += MAX_X_AXIS_DIGIT + 2;

			}
			if(bufferRX[indexOfRXBuffer] == 'y')
			{

				yAxis = bufferRX[indexOfRXBuffer + MAX_Y_AXIS_DIGIT + 2] - '0';
				if(bufferRX[indexOfRXBuffer + 2] == '-') yAxis *= -1;
				indexOfRXBuffer += MAX_Y_AXIS_DIGIT + 2;
			}
			if(bufferRX[indexOfRXBuffer] == 'l')
			{
				leftSignCheck = bufferRX[indexOfRXBuffer + 2] - '0';
				indexOfRXBuffer += 2;
			}
			if(bufferRX[indexOfRXBuffer] == 'r')
			{
				rightSignCheck = bufferRX[indexOfRXBuffer + 2] - '0';
				indexOfRXBuffer += 2;
			}
			if(bufferRX[indexOfRXBuffer] == 'd')
			{
				doorSignCheck = bufferRX[indexOfRXBuffer + 2] - '0';
				indexOfRXBuffer += 2;
			}
			if(bufferRX[indexOfRXBuffer] == '.')
				break;

		indexOfRXBuffer++;

	}

	indicatePosition();
	sendWarning();

    storeCarVariables();
    HAL_UART_Transmit(&huart1, totalTXBuffer, sizeof(totalTXBuffer), 0xFF);
    resetVariables();
}
