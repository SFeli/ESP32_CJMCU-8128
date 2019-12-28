# ESP32_CJMCU-8128
Air Quality with CJMCU-8128 (board with CCS811 / BMP280 and SI702x)

## Hardware:
1) ESP32 (with I2C  4MB and WIFI)
2) CJMCU-8128 - Sensor                   

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
- 
