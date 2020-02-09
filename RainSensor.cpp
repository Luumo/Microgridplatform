#include "RainSensor.h"

RainSensor::RainSensor(int digital_pin, int analog_pin){
    m_digital_pin = digital_pin;
    m_analog_pin = analog_pin;
}


int RainSensor::AnalogSignal(){
    return analogRead(m_analog_pin);
}

int RainSensor::readRain(){
    int range = map(AnalogSignal(), m_min, m_max, 0, 3);
    int rain_level;

    switch(range){
        case 0:
            rain_level = 0; // Max rain
            break;

        case 1:
            rain_level = 1; // medium rain
            break;

        case 2:
            rain_level = 2; // no rain
            break;
    }
    return float(rain_level);
}
