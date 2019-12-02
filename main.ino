#include <arduino.h>
#include "DHT.h"
#include "Digitalpin.h"
#include "Voltagedivider.h"
#include "Currentsensor.h"
#include "Windsensor.h"
#include "RtdSensor.h"
#include "RainSensor.h"

DHT OutdoorTempSensor(48, DHT11);
CurrentSensor Currentsensor1(A7);
VoltageDivider voltagedivider1(A1, 1040000.0, 222500.0);
// WindSensor windsensor(A3);
RtdSensor RtdSensor1(A3);
RainSensor Rainsensor1(52, 15);


void setup() {          
  Serial.begin(115200);
  OutdoorTempSensor.begin();
}

void loop() {
  Serial.println("----- Outdoor Temp Sensor -----");
  float h = OutdoorTempSensor.readHumidity();
  float t = OutdoorTempSensor.readTemperature();

  Serial.print("Temp: ");
  Serial.println(t);
  Serial.print("Humidity: ");
  Serial.println(h);

  Serial.println("----- Rain Sensor -----");
  int rain = Rainsensor1.SenseRain();
  int analog_rain = Rainsensor1.AnalogSignal();

Serial.print("Rain: ");
  if (rain == 1){
    Serial.println("Dry");
    }
    else{
      Serial.println("Wet");
    }
  Serial.print("Analog signal: ");
  Serial.println(analog_rain);

  Serial.println("----- PT 1000 Sensor -----");
  RtdSensor1.calcTemperature();

  Currentsensor1.calcCurrentValue();
  voltagedivider1.calculateVin();
  delay(1000);
}
