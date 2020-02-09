#include <Arduino.h>
#line 34 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino"
void setup();
#line 41 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino"
void loop();
#line 46 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino"
void dataTransfer(int delayTime);
#line 0 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino"
#line 1 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\Currentsensor.cpp"
#include "Currentsensor.h"

CurrentSensor::CurrentSensor(int pin){
    m_pin = pin;
}

void CurrentSensor::getAdcValue(int _pin){
    m_adcValue = analogRead(_pin);
}

 void CurrentSensor::calcAdcVoltage(){
    m_adcVoltage = (m_adcValue / 1024.0) * 5000; // returns adc voltage
}

float CurrentSensor::readCurrent(){
    getAdcValue(m_pin);
    calcAdcVoltage();
    currentValue = ((m_adcVoltage - m_offsetVoltage) / m_sensitivity);
    return currentValue;
}
#line 1 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino"
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

VoltageDivider  solarPanelVoltage(A0, 215000.0, 66500.0);
VoltageDivider  batteryVoltage(A1, 217000.0, 66100.0);

CurrentSensor   solarPanelCurrent(10);
CurrentSensor   batteryCurrent(11);
CurrentSensor   loadCurrent(12);

RtdSensor       batteryTemp(A3, float(21)); //offset about 21 ohm
RtdSensor       solarPanelTemp(A4, float(28)); //ofset about 28 ohm (4 ohm = 1 degree celcius)

RainSensor      Rainsensor(52, 15);
DHT             OutdoorTempSensor(A6, DHT11);
WindSensor      windSensor(14);

// Function declaration
void serialPrintSensorData(String location, String sensorname, float value, String prefix);
bool serialReadyCheck();
void dataTransfer(int delay);

// Main program
void setup() {          
  Serial.begin(115200);
  OutdoorTempSensor.begin();
  serialReadyCheck();
  delay(100);
}

void loop(){
  dataTransfer(2000);
}

// Functions
void dataTransfer(int delayTime){

  /*
  reads sensordata and sends it via serial communication.
  each value is separated by a comma.
  Decodes on the recieving RPI
  structure:
  clusterlocation, sensortype, value, prefix
  */
  float solarpanelvoltage   = solarPanelVoltage.readVoltage();      delay(25);
  float batteryvoltage      = batteryVoltage.readVoltage();         delay(25);

  float solarpanelcurrent  = solarPanelCurrent.readCurrent();       delay(25);
  float batterycurrent     = batteryCurrent.readCurrent();          delay(25);
  float loadcurrent        = loadCurrent.readCurrent();             delay(25);

  float batterytemp         = batteryTemp.readTemperature();        delay(25);
  float solarpaneltemp      = solarPanelTemp.readTemperature();     delay(25);

  int rain                  = Rainsensor.readRain();                delay(25);
  float outdoorhumidity     = OutdoorTempSensor.readHumidity();     delay(25);
  float outdoortemperature  = OutdoorTempSensor.readTemperature();  delay(25);
  float windspeed           = windSensor.readWindSpeed();           delay(25);

  serialPrintSensorData(LOCATION, "SPV", solarpanelvoltage, "V");
  serialPrintSensorData(LOCATION, "BV", batteryvoltage, "V");

  serialPrintSensorData(LOCATION, "SPC", solarpanelcurrent, "A");
  serialPrintSensorData(LOCATION, "BC", batterycurrent, "A");
  serialPrintSensorData(LOCATION, "LC", loadcurrent, "A");

  serialPrintSensorData(LOCATION, "BT", batterytemp, "celcius");
  serialPrintSensorData(LOCATION, "SPT", solarpaneltemp, "celcius");

  serialPrintSensorData(LOCATION, "RAIN", rain, "HIGH/MEDIUM/LOW");
  serialPrintSensorData(LOCATION, "OUTHUM", outdoorhumidity, "%");
  serialPrintSensorData(LOCATION, "OUTTEMP", outdoortemperature, "celcuis");
  serialPrintSensorData(LOCATION, "windspeed", windspeed, "m/s");

  delay(delayTime);
}

void serialPrintSensorData(String location, String sensorname, float value, String prefix){
  Serial.print(location);
  Serial.print(",");
  Serial.print(sensorname);
  Serial.print(",");
  Serial.print(value);
  Serial.print(",");
  Serial.print(prefix);
  Serial.print("\n");
}

bool serialReadyCheck(){
  // Wait until RPI is ready
  while (1){
    if (Serial.available() > 0){
      String data = Serial.readStringUntil('\n');

      if (data.equals("RPIready")){
        return true;
        break;
      }
    }
  }
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
