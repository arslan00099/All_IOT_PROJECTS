#include <Adafruit_BMP280.h>
#include "bmp280_sensor.h"

Adafruit_BMP280  bmp280;


boolean tempSensorInit() {
  if (!bmp280.begin(BMP280_I2C_ADDRESS)) {
    // error BMP280
    return false;
  }
  else {
    /* Default settings from datasheet. */
    bmp280.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                       Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                       Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                       Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                       Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
    return true;
  }
}

 float readTemp(char* tempStr){
  //char _buffer[10] = "";
  float temp = 0;
  temp = bmp280.readTemperature();
  if (temp < 0)   // if temperature < 0
        sprintf( tempStr, " -%02u.%02u%cC", (int)abs(temp), (int)(abs(temp) * 100) % 100, 247);
  else            // temperature >= 0
        sprintf(tempStr, "  %02u.%02u%cC ", (int)temp, (int)(temp * 100) % 100, 247);
  return temp;
}
