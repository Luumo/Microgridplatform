#include "RainSensor.h"
#include "Sampling.h"

RainSensor::RainSensor(int digital_pin, int analog_pin){
    m_digital_pin = digital_pin;
    m_analog_pin = analog_pin;
    pinMode(m_digital_pin, OUTPUT);
}


int RainSensor::readAnalogSignal(){
    analogRead(m_analog_pin);  // dummy read
    analogRead(m_analog_pin);  // dummy read
    int analog_signal_mean = data_sampling(analogRead(m_analog_pin), 10);
    return analog_signal_mean;
}

int RainSensor::readRain(){
    digitalWrite(m_digital_pin, HIGH);
    delay(10);
    int rain = map(readAnalogSignal(), m_min, m_max, 1023, 0);
    digitalWrite(m_digital_pin, LOW);
    return rain;
}

int RainSensor::readMappedRain(){
    int range = map(readAnalogSignal(), m_min, m_max, 0, 3);
    int m_rain_level = 0;

    switch(range){
        case 0:
            m_rain_level = 2; // Max rain
            break;

        case 1:
            m_rain_level = 1; // medium rain
            break;

        case 2:
            m_rain_level = 0; // no rain
            break;
    }
    return m_rain_level;
}