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