#include "main.h"
								/*** Definitions. ***/
// Maximum digits macros.
#define MAX_SPEED_DIGIT 3
#define MAX_ACCERLATION_DIGIT 2
#define MAX_X_AXIS_DIGIT 1
#define MAX_Y_AXIS_DIGIT 1

// TX and RX size macros.
#define TX_SIZE 50
#define RX_SIZE 50

// Car locations macros.
#define EAST 1
#define WEST 2
#define NORTH 3
#define SOUTH 4
#define NORTH_EAST 5
#define NORTH_WEST 6
#define SOUTH_EAST 7
#define SOUTH_WEST 8

// Car doors macros.
#define LEFT_DOOR  1
#define RIGHT_DOOR 2
#define BAG_DOOR   3
#define CAR_HOOD   4


								/*** Functions Prototypes. ***/
uint8_t calculateValue(int indexOfRXBuffer);
uint8_t takeAbsoulute(int8_t value);
void convertIntToChars(uint8_t maxDigit, uint8_t value);
void indicatePosition();
void sendWarning();
void storeCarVariables();
void resetVariables();
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
