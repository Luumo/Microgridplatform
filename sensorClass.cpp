#include "sensorClass.h"
#include "Sampling.h"

Sensor::Sensor(int pin){
    m_pin = pin;
}

float Sensor::readPin(){
    analogRead(m_pin); // dummy read
    analogRead(m_pin); // dummy read

    return data_sampling(analogRead(m_pin), 10)

}