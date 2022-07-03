

typedef struct DataGPS
{
	float longitude;
	float latitude;
	char dirLongitude;
	char dirLatitude;
	char valid;
}gps_t;


void getGPSData(void);
