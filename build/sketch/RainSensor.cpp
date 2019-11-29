#include "RainSensor.h"

RainSensor::RainSensor(int digital_pin, int analog_pin){
    m_digital_pin = digital_pin;
    m_analog_pin = analog_pin;
}