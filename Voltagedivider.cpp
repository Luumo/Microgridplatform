#include "Voltagedivider.h"
#include "Sampling.h"

VoltageDivider::VoltageDivider(int pin, float R1, float R2){
    m_pin = pin;
    m_R1 = R1;
    m_R2 = R2;
}

void VoltageDivider::readInputSignal(int _pin){
    m_inputvalue = data_sampling(analogRead(m_pin), 20);
}


float VoltageDivider::readVoltage(){
    readInputSignal(m_pin);
    float voltage = m_inputvalue * (5.0 / 1023.0);
    voltage = voltage / (m_R2/(m_R1+m_R2));

    return voltage;
}
