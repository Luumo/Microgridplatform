#include "RtdSensor.h"
#include "Sampling.h"

RtdSensor::RtdSensor(int pin, float resistance_offset){
    m_pin = pin;
    m_resistance_offset = resistance_offset;
}

float RtdSensor::readInputSignal(){
    analogRead(m_pin); // dummy read
    analogRead(m_pin); // dummy read
    m_ADC = data_sampling(analogRead(m_pin), 10);
}


float RtdSensor::getResistance(){
    // read sampled inputdata
    readInputSignal();
    //calculate resistance of pt-1000 thermistor
    float resistance = 5000.0 / ( (1023.0 / m_ADC) -1);
    //thermistor offset
    resistance += m_resistance_offset;

    m_resistance = resistance;
}

float RtdSensor::readTemperature(){
    getResistance();
    float temp =((-(R0 * A) +  (sqrt( (R0*A)*(R0*A) + (-4*R0*B*(R0-m_resistance) ) ) ) ) / (2*R0*B)); // formula from PT-1000 doc.
    float offset = (0.30 + 0.005*temp);
    return temp;

}


