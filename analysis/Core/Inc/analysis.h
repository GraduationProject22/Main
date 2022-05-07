#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "usart.h"

#define RX_SIZE 100
#define TX_SIZE 100
#define PI 3.14159265
#define DX_MIN 3
#define DY_MIN 3
#define TEMP_ARR_SIZE 20
#define MAX_FLOAT_DIGITS 9

typedef struct CarData
{
	float longitude;
	float latitude;
	float speed;
	float Ax;
	float Ay;
	float angle;
	float Vx;
	float Vy;
	float dx;
	float dy;
	float x2;
	float y2;
}data_t;

void resetBuffersIndexes(void);
float splitData(void);
void getNeighborData(void);
void getCarData(void);
float getAbsolute(float val);
void sendWarning(void);
void analysis(void);
void mergeData(float value);
void generateTransmitBuffer(void);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
