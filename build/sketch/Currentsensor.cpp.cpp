#include <Arduino.h>
#line 18 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino"
void setup();
#line 23 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino"
void loop();
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

double CurrentSensor::calcCurrentValue(){
    getAdcValue(m_pin);
    calcAdcVoltage();
    currentValue = ((m_adcVoltage - m_offsetVoltage) / m_sensitivity);

    Serial.println("-----Current Sensor -----" );
    Serial.print("Raw Sensor Value = " );
    Serial.println(m_adcValue);
    Serial.print(" Voltage (mV)= " );
    Serial.println(m_adcVoltage);
    Serial.print(" Current = " );
    Serial.print(currentValue);
    Serial.println("\n"); 
    return currentValue;
}
#line 1 "c:\\Users\\LUMO\\Desktop\\Exjobb\\Software\\Microgridplatform\\main.ino"
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

