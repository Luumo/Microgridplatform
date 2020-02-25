#include <Arduino.h>
#line 38 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino"
void setup();
#line 44 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino"
void loop();
#line 49 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino"
void dataTransfer(int delayTime);
#line 0 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino"
#line 1 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\Currentsensor.cpp"
#include "Currentsensor.h"
#include "Sampling.h"

CurrentSensor::CurrentSensor(int pin, float sensitivity){
    m_pin = pin;
    m_sensitivity = sensitivity;
}

void CurrentSensor::getAdcValue(int _pin){
    analogRead(m_pin); // dummy read
    analogRead(m_pin); // dummy read
    m_adcValue = data_sampling(analogRead(_pin), 20);
}

 void CurrentSensor::calcAdcVoltage(){
    m_adcVoltage = (m_adcValue / 1024.0) * 5000.0; // returns adc voltage
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
#include "Sampling.h"

#define LOCATION "Roof"    // Sensorcluster location

VoltageDivider  solarPanelVoltage(A0, 216000.0, 66500.0);
VoltageDivider  batteryVoltage(A1, 217000.0, 66100.0);
// VoltageDivider  loadVoltage(A2, 216000.0, 66100.0); // not used

CurrentSensor   solarPanelCurrent(10, 100.0);
CurrentSensor   batteryCurrent(11, 100.0);
CurrentSensor   loadCurrent(12, 100.0);

RtdSensor       batteryTemp(A3, float(20.81)); //offset about 21 ohm
RtdSensor       solarPanelTemp(A4, float(28.0)); //ofset about 28 ohm (4 ohm = 1 degree celcius)

DHT             OutdoorTempSensor(A6, DHT11);
WindSensor      windSensor(14);
RainSensor      Rainsensor(48, 15);



// Function declaration
void serialPrintSensorData(String location, String sensorname, float value, String prefix);
bool serialReadyCheck();
void dataTransfer(int delay);
float stateOfCharge(float voltage, float voltage_max, float voltage_min);

// setup
void setup() {          
  Serial.begin(115200);
  OutdoorTempSensor.begin();
  delay(100);
}
// main
void loop(){
  dataTransfer(3000);
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

  float solarpanelvoltage   = solarPanelVoltage.readVoltage();
  float batteryvoltage      = batteryVoltage.readVoltage();

  float solarpanelcurrent   = solarPanelCurrent.readCurrent();
  float batterycurrent      = batteryCurrent.readCurrent();
  float loadcurrent         = loadCurrent.readCurrent();

  float batterytemp         = batteryTemp.readTemperature();
  float solarpaneltemp      = solarPanelTemp.readTemperature();

  float outdoorhumidity     = OutdoorTempSensor.readHumidity();
  float outdoortemperature  = OutdoorTempSensor.readTemperature();
  float windspeed           = windSensor.readWindSpeed();
  int   rain                  = Rainsensor.readRain();

  float SoC = stateOfCharge(batteryVoltage.m_voltage, 12.6, 9.6);

  serialPrintSensorData(LOCATION, "SPV", solarpanelvoltage, "V");
  serialPrintSensorData(LOCATION, "BV", batteryvoltage, "V");
  serialPrintSensorData(LOCATION, "SoC", SoC, "%");

  // serialPrintSensorData(LOCATION, "LV", loadvoltage, "V");

  serialPrintSensorData(LOCATION, "SPC", solarpanelcurrent, "A");
  serialPrintSensorData(LOCATION, "BC", batterycurrent, "A");
  serialPrintSensorData(LOCATION, "LC", loadcurrent, "A");

  serialPrintSensorData(LOCATION, "BT", batterytemp, "celcius");
  serialPrintSensorData(LOCATION, "SPT", solarpaneltemp, "celcius");

  serialPrintSensorData(LOCATION, "OUTHUM", outdoorhumidity, "%");
  serialPrintSensorData(LOCATION, "OUTTEMP", outdoortemperature, "celcuis");
  serialPrintSensorData(LOCATION, "windspeed", windspeed, "m/s");
  serialPrintSensorData(LOCATION, "RAIN", rain, "HIGH/MEDIUM/LOW");


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

float stateOfCharge(float voltage, float voltage_max, float voltage_min){
  /*
  Calculates state of charge of battery and returns percentage left
  @param voltage is actual voltage level of the battery
  @param voltage_max is the voltage when battery is fully charged
  @param voltage_min is the minimum voltage of battery
  */
  float SoC = ((voltage - voltage_min) / (voltage_max - voltage_min)) * 100;
  if (SoC < 0){
    SoC = 0;
  }
  return SoC; 
}
