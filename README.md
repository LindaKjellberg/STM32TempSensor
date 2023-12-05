# Portfolioarbete STM32TempSensor

## Use case

Att skapa en D2C- (Device to Cloud) lösning som ska samla in mät-data från en DHT22 (temperatur- och luftfuktighetssensor) kopplad till ESP32 som skicka datat över WIFI till en AWS IoT Core, samt spara datat i en databas.


### _Hårdvaru-komponenter:_

- DHT22 (AM2302)
- ESP32 microcontroller med WIFI
- 10 kOhm resistor
- Strömkälla (USB kabel)
- Jumperkablar

### _Kopplingsschema:_

- (+) 3.3V pin till DHT22 pin 1
- (-) GND till DHT22 pin 4
- GPIO D4 med 10kOhm resistor till DHT22 pin 2
- DHT22 pin 3 är inte kopplad

### _Mjukvarukomponenter:_
- Arduino IDE
- AWS IoT Core
- TimeStreamDB
- Grafana


### Setup