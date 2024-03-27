#ifndef BMP280_SENSOR_H
#define BMP280_SENSOR_H

#define BMP280_I2C_ADDRESS  0x76
boolean tempSensorInit(void);
//char* readTemp(void);
float readTemp(char* tempStr);
#endif
