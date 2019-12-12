#include <arduino.h>
#include "DHT.h"
#include "Digitalpin.h"
#include "Voltagedivider.h"
#include "Currentsensor.h"
#include "Windsensor.h"
#include "RtdSensor.h"
#include "RainSensor.h"


#define LOCATION 1

DHT OutdoorTempSensor(48, DHT11);
CurrentSensor Currentsensor1(A7);
VoltageDivider voltagedivider1(A1, 1040000.0, 222500.0);
RtdSensor RtdSensor1(A3);
RainSensor Rainsensor1(52, 15);
// WindSensor windsensor(A3);



void setup() {          
  Serial.begin(115200);
  OutdoorTempSensor.begin();
}

void loop() {

  // line protocol: LOCATION,sensortype, timestamp, value


  Serial.print(LOCATION); 
  Serial.print(",");
  Serial.print("OutdoorTempSensor");
  Serial.print(",");
  Serial.print(OutdoorTempSensor.readTemperature());
  Serial.print("\n");
  delay(1000);
}

/*

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
*/

/*
Schema for InfluxDX:
See Line Protocol Syntax for influxDB

data to be sent from arduino:
clusterlocation,sensortype, value, date
->
this has to be parsed to python:
data = [
  {
    "measurement": sensorcluster,
    "tags": {
      "location": location_var,
      "sensortype": sensortype_Var
    },
    "time": iso,
    "fields": {
      "value": sensorvalue_Var
    }
  }
]


**SORTED BY SENSORCLUSTER**
ex:

INSERT sensorcluster,clusterlocation=1,sensortype=outdoortemp value=23 XXXXXXXXXXXX 
see Milk-sensor documentation for futher info about data protocol from python

name: Sensorcluster
-----------------
time                         sensortype       clusterlocation    value
2019-12-11T19:30:00.xxxxxx   Humidity         1                    90
2019-12-11T19:30:00.xxxxxx   outdoortemp      1                    22
2019-12-11T19:30:00.xxxxxx   batterytemp      1                    23
2019-12-11T19:30:00.xxxxxx   batteryCurrent   1                    8
2019-12-11T19:30:00.xxxxxx   batteryvoltage   1                    12.3
2019-12-11T19:30:00.xxxxxx   PVvoltage        1                    12.3
2019-12-11T19:30:00.xxxxxx   PVCurrent        1                    8


*/