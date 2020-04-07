/* ESP32
    Sensor CJMCU-8128 mit:
    CCS811 -> 0x5A !!!
      bool begin()
      bool start(mode)
      read(eco2, etvoc, errstat, raw)
      int hardware_version()
      int bootloader_version()
      int application_version()
      int get_errorid()
      bool set_envdate(t,h)
      bool set_envdata210(t,h)
      bool get_baseline(baseline)
      bool set_baseline(baseline)
      bool flash(image, size)
      set_i2cdelay(us)
      get_i2cdelay(us)
      wake_init()
      wake_up()
      wake_down()
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
#include "ccs811.h"                     // include library for CCS811 - Sensor from martin-pennings https://github.com/maarten-pennings/CCS811
#include "Adafruit_Si7021.h"            // include main library for SI7021 - Sensor
#include "Adafruit_BMP280.h"            // include main library for BMP280 - Sensor
#include <Wire.h>                       // This library allows you to communicate with I2C

CCS811 ccs811;
Adafruit_BMP280 bmp280;                // I2C
Adafruit_Si7021 SI702x = Adafruit_Si7021();

void setup() {
  Serial.begin(9600);
  // Enable I2C
  Wire.begin(21,22);                  // make TTGO boards run with this code
  Serial.println("CCS811 test");      /* --- SETUP CCS811 on 0x5A ------ */
  ccs811.set_i2cdelay(50); // Needed for ESP8266 because it doesn't handle I2C clock stretch correctly
  if (!ccs811.begin()) {
    Serial.println("Failed to start sensor! Please check your wiring.");
    while (true);
  }
  // Print CCS811 versions
  Serial.print("setup: hardware    version: "); Serial.println(ccs811.hardware_version(), HEX);
  Serial.print("setup: bootloader  version: "); Serial.println(ccs811.bootloader_version(), HEX);
  Serial.print("setup: application version: "); Serial.println(ccs811.application_version(), HEX);

  // Start measuring
  bool ok = ccs811.start(CCS811_MODE_1SEC);
  if ( !ok ) Serial.println("setup: CCS811 start FAILED");

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

  uint16_t eco2, etvoc, errstat, raw;     // Read CCS811

  ccs811.set_envdata(bmp280.readTemperature(), SI702x.readHumidity());
  ccs811.read(&eco2, &etvoc, &errstat, &raw);
  if ( errstat == CCS811_ERRSTAT_OK ) {
    Serial.print("CCS811 => CO2 = ");
    Serial.print(eco2);
    Serial.print("ppm, TVOC = ");
    Serial.println(etvoc);
  }
  delay(5000);
}
