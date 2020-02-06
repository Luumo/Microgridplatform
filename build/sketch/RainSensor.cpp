#include "RainSensor.h"

RainSensor::RainSensor(int digital_pin, int analog_pin){
    m_digital_pin = digital_pin;
    m_analog_pin = analog_pin;
    pinMode(m_digital_pin, INPUT);
    pinMode(m_analog_pin, INPUT);
}

int RainSensor::readRain(){
    if (digitalRead(m_digital_pin) == LOW){
        return 0; // Wet
    }
    else{
        return 1; // Dry
    }
}

int RainSensor::AnalogSignal(){
    return analogRead(m_analog_pin);
}
