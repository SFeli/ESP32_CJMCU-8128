# ESP32_CJMCU-8128
Air Quality with CJMCU-8128 (board with CCS811 / BMP280 and SI702x)
- TVCO (Total Volatile Organic Compound)
- eCO2 (equivalent CO2)
- Humidity (measured by SI702x)
- Temperature (measured by BMP280 and SI702x)
- Pressure (measured by BMP280)

## Hardware:
1) ESP32 (with I²C  4MB and WIFI)
2) CJMCU-8128 - Sensor
    More information about the Core-Sensor CCS811 from AMS can be found under: https://ams.com/environmental-sensors

## Wireing:

| MH-Z19b |   |    ESP32  |
| --- | -- | -----------|
| VCC  | <-> | 3V3 |
| GND |  <->| GND |
| SDA   | <-> | SDA Pin 21 |
| SCL   | <-> | SCL Pin 22 |
| WAK   | <-> | GND |

## Implementation
- Code is raw without any additional features
- Output on serial monitor 9600 Baud
