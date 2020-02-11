#include "Currentsensor.h"
#include "Sampling.h"

CurrentSensor::CurrentSensor(int pin, float sensitivity){
    m_pin = pin;
    m_sensitivity = sensitivity;
}

void CurrentSensor::getAdcValue(int _pin){
    m_adcValue = data_sampling(analogRead(_pin), 10);
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