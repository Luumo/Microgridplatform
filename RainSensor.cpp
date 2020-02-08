#include "RainSensor.h"

RainSensor::RainSensor(int digital_pin, int analog_pin){
    m_digital_pin = digital_pin;
    m_analog_pin = analog_pin;
    pinMode(m_digital_pin, INPUT);
    pinMode(m_analog_pin, INPUT);
}


int RainSensor::AnalogSignal(){
    Serial.println(analogRead(m_analog_pin));
    return analogRead(m_analog_pin);
}

int RainSensor::readRain(){
    int range = map(AnalogSignal(), m_min, m_max, 0, 3);

    switch(range){
        case 0:
            return 0; // Max rain
            break;

        case 1:
            return 1; // medium rain
            break;

        case 2:
            return 2; // no rain
            break;
    }
}
