#include "RainSensor.h"
#include "Sampling.h"

RainSensor::RainSensor(int digital_pin, int analog_pin){
    m_digital_pin = digital_pin;
    m_analog_pin = analog_pin;
}


int RainSensor::readAnalogSignal(){
    int analog_signal_mean = data_sampling(analogRead(m_analog_pin), 10);
    return analog_signal_mean;
}

int RainSensor::readRain(){
    int range = map(readAnalogSignal(), m_min, m_max, 0, 3);
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
