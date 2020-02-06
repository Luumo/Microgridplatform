#include <arduino.h>
#include <string.h>
#include "DHT.h"
#include "Voltagedivider.h"
#include "Currentsensor.h"
#include "Windsensor.h"
#include "RtdSensor.h"
#include "RainSensor.h"
#include "Windsensor.h"
#include "Sensor.h"

#define LOCATION "Roof"    // Sensorcluster location

VoltageDivider  solarPanelVoltage(A0, 100000.0, 47000.0);
VoltageDivider  batteryVoltage(A1, 100000.0, 47000.0);

CurrentSensor   solarPanelCurrent(10);
CurrentSensor   batteryCurrent(11);
CurrentSensor   loadCurrent(12);

RtdSensor       batteryTemp(A3);
RtdSensor       solarPanelTemp(A4);

RainSensor      Rainsensor(52, 15);
DHT             OutdoorTempSensor(A6, DHT11);
WindSensor      windSensor(14);

/*
Make sure listOfSensors consists of same sensors as dataTransfer function.
each name in listOfSensors should end with a comma, except the last one
  structure:
  "Sensorname1, Sensorname2, Sensorname3,...,SensornameN"
*/
/*
https://www.arduino.cc/reference/tr/language/variables/data-types/string/
*/


void serialPrintSensorData(char *location, char *sensorname, float value, char *prefix){
  Serial.print(location);
  Serial.print(",");
  Serial.print(sensorname);
  Serial.print(",");
  Serial.print(value);
  Serial.print(",");
  Serial.print(prefix);
  Serial.print("\n");
}

void dataTransfer(int delayTime){

  /*
  reads sensordata and sends it via serial communication.
  each value is separated by a comma.
  Decodes on the recieving RPI
  structure:
  clusterlocation
  sensortype, value, prefix
  */
  float solarpanelvoltage   = solarPanelVoltage.readVoltage();
  float batteryvoltage      = batteryVoltage.readVoltage();

  double solarpanelcurrent  = solarPanelCurrent.readCurrent();
  double batterycurrent     = batteryCurrent.readCurrent();
  double loadcurrent        = loadCurrent.readCurrent();

  float batterytemp         = batteryTemp.readTemperature();
  float solarpaneltemp      = solarPanelTemp.readTemperature();

  int rain                  = Rainsensor.readRain();
  float outdoorhumidity     = OutdoorTempSensor.readHumidity();
  float outdoortemperature  = OutdoorTempSensor.readTemperature();
  float windspeed           = windSensor.readWindSpeed();

  serialPrintSensorData(LOCATION, "SPV", solarpanelvoltage, "V");
  serialPrintSensorData(LOCATION, "BV", batteryvoltage, "V");

  serialPrintSensorData(LOCATION, "SPC", solarpanelcurrent, "A");
  serialPrintSensorData(LOCATION, "BC", batterycurrent, "A");
  serialPrintSensorData(LOCATION, "LC", loadcurrent, "A");

  serialPrintSensorData(LOCATION, "BT", batterytemp, "celcius");
  serialPrintSensorData(LOCATION, "SPT", solarpaneltemp, "celcius");

  serialPrintSensorData(LOCATION, "RAIN", rain, "HIGH/LOW");
  serialPrintSensorData(LOCATION, "OUTHUM", outdoorhumidity, "V");
  serialPrintSensorData(LOCATION, "OUTTEMP", outdoortemperature, "V");
  serialPrintSensorData(LOCATION, "windspeed", windspeed, "m/s");
}

void CheckConnectionRPI(){
  // wait for serial port to connect.
  while (!Serial) {
  }
}
void setup() {          
  Serial.begin(115200);
  OutdoorTempSensor.begin();

  CheckConnectionRPI();
  delay(100);

}

void loop(){

dataTransfer(5000);

/*
  int solarcurrent = solarPanelCurrent.readCurrent();
  Serial.print(" solarcurrent = " );
  Serial.println(solarcurrent);

  int batterycurrent = batteryCurrent.readCurrent();
  Serial.print(" batterycurrent = " );
  Serial.println(solarcurrent);

  int loadcurrent = loadCurrent.readCurrent();
  Serial.print(" batterycurrent = " );
  Serial.println(solarcurrent);

  delay(1000);
*/
}


/*
Schema for InfluxDX:
See Line Protocol Syntax for influxDB
handled at raspberry:
  date, time

data to be sent from arduino:
  clusterlocation,sensortype, value

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