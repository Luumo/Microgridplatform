#include "Voltagedivider.h"
#include "Sampling.h"

VoltageDivider::VoltageDivider(int pin, float R1, float R2){
    m_pin = pin;
    m_R1 = R1;
    m_R2 = R2;
}

void VoltageDivider::readInputSignal(int _pin){
    analogRead(m_pin);  // dummy read
    analogRead(m_pin);  // dummy read
    m_inputvalue = data_sampling(analogRead(m_pin), 10);
}


float VoltageDivider::readVoltage(){
    readInputSignal(m_pin);
    m_voltage = m_inputvalue * (5.0 / 1023.0);
    m_voltage = m_voltage / (m_R2/(m_R1+m_R2));

    return m_voltage;
}
