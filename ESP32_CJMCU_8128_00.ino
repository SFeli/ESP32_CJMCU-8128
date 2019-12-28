/* Lolin D32
    Sensor CJMCU 8128 mit:
    CCS811 -> 0x5A !!!
      bool begin(addr)
      setDriverMode(Mode 1sec, 10sec, 60 sec, 250 msec)
      bool available()
      setEnvironmentalData(humidity, temperature)
      double calculateTemperature()
      setThresholds(low,med,hysteresis)
      SWReset()
      bool checkError()
      _i2c_init()
      setTempOffset(float) ?
    SI7021 -> 0x40
      float readHumidity()
      float readTemperature()
      _readRevision
      readSerialNumber()
      reset()
    BMP280 -> 0x76
      bool (begin(addr)
      setSampling(mode)
      readCoefficients()
      float readTemperature()
      float readPressure()
      float readAltitude(seaLevelhPa)
      float seaLevelForAltitude
      takeForcedMeasurement()
*/

#include <Arduino.h>
#include "Adafruit_CCS811.h"            // include main library for CCS811 - Sensor
#include "Adafruit_Si7021.h"            // include main library for SI7021 - Sensor
#include "Adafruit_BMP280.h"            // include main library for BMP280 - Sensor
#include <Wire.h>                       // This library allows you to communicate with I2C

Adafruit_CCS811 ccs811;
Adafruit_BMP280 bmp280;                // I2C
Adafruit_Si7021 SI702x = Adafruit_Si7021();

void setup() {
  Serial.begin(9600);

  Serial.println("CCS811 test");      /* --- SETUP CCS811 on 0x5A ------ */
  if (!ccs811.begin(0x5A)) {
    Serial.println("Failed to start sensor! Please check your wiring.");
    while (true);
  }
  // Wait for the sensor to be ready
  while (!ccs811.available());

  Serial.println("BMP280 test");     /* --- SETUP BMP on 0x76 ------ */
  if (!bmp280.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (true);
  }

  Serial.println("Si7021 test!");     /* ---- SETUP SI702x ----- */
  if (!SI702x.begin()) {
    Serial.println("Did not find Si702x sensor!");
    while (true);
  }
  Serial.print("Found model ");
  switch (SI702x.getModel()) {
    case SI_Engineering_Samples:
      Serial.print("SI engineering samples"); break;
    case SI_7013:
      Serial.print("Si7013"); break;
    case SI_7020:
      Serial.print("Si7020"); break;
    case SI_7021:
      Serial.print("Si7021"); break;
    case SI_UNKNOWN:
    default:
      Serial.print("Unknown");
  }
  Serial.print(" Revision(");
  Serial.print(SI702x.getRevision());
  Serial.print(")");
  Serial.print(" Serial #"); Serial.print(SI702x.sernum_a, HEX); Serial.println(SI702x.sernum_b, HEX);
}

void loop() {
  Serial.print("BMP280 => Temperature = ");
  Serial.print(bmp280.readTemperature());
  Serial.print(" °C, ");

  Serial.print("Pressure = ");
  Serial.print(bmp280.readPressure() / 100);
  Serial.println(" Pa, ");

//  Serial.print("Approx altitude = ");
//  Serial.print(bmp280.readAltitude(1013.25));        /* Adjusted to local forecast! */
//  Serial.println(" m");

  Serial.print("SI702x => Temperature = ");
  Serial.print(SI702x.readTemperature(), 2);
  Serial.print(" °C, ");
  Serial.print("Humidity = ");
  Serial.println(SI702x.readHumidity(), 2);

  ccs811.setEnvironmentalData(SI702x.readHumidity(), bmp280.readTemperature());
  if (!ccs811.readData()) {
    Serial.print("CCS811 => CO2 = ");
    Serial.print(ccs811.geteCO2());
    Serial.print("ppm, TVOC = ");
    Serial.println(ccs811.getTVOC());
  }
  delay(5000);
}
