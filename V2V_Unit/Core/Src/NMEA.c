#include "NMEA.h"
#include <stdlib.h>
#include <stdio.h>
extern char RMC[100];

gps_t gpsData;
uint16_t indexRMC = 0;
extern uint16_t indexTempArray;
extern char tempArray[20];
extern int globalIndex;
int count = 0;

void getGPSData(void)
{
	while(RMC[indexRMC] == ',') indexRMC++;
	while(RMC[indexRMC] != ',') indexRMC++;

	while(RMC[indexRMC] == ',') indexRMC++;
	gpsData.valid = RMC[indexRMC++];

	if(gpsData.valid == 'V' || gpsData.valid == 'N')
	{
		/* Get latitude */
		indexTempArray = 0;
		tempArray[indexTempArray++] = '0';
		tempArray[indexTempArray++] = '.';
		while(RMC[indexRMC] == ',') indexRMC++;
		while(RMC[indexRMC] != ',')
		{
			if(count == 0)
			{
				if(RMC[indexRMC] == '0') count--;
				gpsData.latitude = (RMC[indexRMC] - '0') * 10;
			}
			else if(count == 1)
			{
				gpsData.latitude += (RMC[indexRMC] - '0');
			}
			else
			{
				if(RMC[indexRMC] != '.')
					tempArray[indexTempArray] = RMC[indexRMC];
				indexTempArray++;

			}
			count++;
			indexRMC++;
		}
		gpsData.latitude += (atof(tempArray) / 60);
		for(globalIndex = 0; globalIndex < 20; globalIndex++)
			tempArray[globalIndex] = '0';
		count = 0;


		/* Get latitude direction */
		while(RMC[indexRMC] == ',') indexRMC++;
		gpsData.dirLatitude = RMC[indexRMC++];

		/* Get longitude */
		indexTempArray = 0;
		tempArray[indexTempArray++] = '0';
		tempArray[indexTempArray++] = '.';
		while(RMC[indexRMC] == ',') indexRMC++;
		while(RMC[indexRMC] != ',')
		{
			if(count == 0)
			{
				if(RMC[indexRMC] == '0') count--;
				gpsData.longitude = (RMC[indexRMC] - '0') * 10;
			}
			else if(count == 1)
			{
				gpsData.longitude += (RMC[indexRMC] - '0');
			}
			else
			{
				if(RMC[indexRMC] != '.')
					tempArray[indexTempArray] = RMC[indexRMC];
				indexTempArray++;

			}
			count++;
			indexRMC++;
		}
		gpsData.longitude += (atof(tempArray) / 60);
		for(globalIndex = 0; globalIndex < 20; globalIndex++)
				tempArray[globalIndex] = '0';
		count = 0;

		/* Get longitude direction */
		while(RMC[indexRMC] == ',') indexRMC++;
		gpsData.dirLongitude = RMC[indexRMC++];

		indexRMC = 0;
		indexTempArray = 0;
	}
	indexTempArray = 0;

}

