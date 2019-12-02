#include "Voltagedivider.h"

VoltageDivider::VoltageDivider(int pin, float R1, float R2){
    m_pin = pin;
    m_R1 = R1;
    m_R2 = R2;
}

void VoltageDivider::readInputValue(int _pin){
   m_inputvalue = analogRead(m_pin);
}


float VoltageDivider::calculateVin(){
    readInputValue(m_pin);
    float voltage = m_inputvalue * (4.998 / 1023.0);
    voltage = voltage / (m_R2/(m_R1+m_R2));

    Serial.println("-----Voltage Sensor -----" );
    Serial.print("Voltage: ");
    Serial.print(voltage);
    Serial.println(" V");

}

/*
float VoltageDivider::calculateVin(){
    readInputValue(m_pin);
    m_vout = (m_inputvalue * 5) / 1024.0; // 5 might have to be changed
    vin = (m_vout * m_R2) / (m_R1 + m_R2));
    m_vout = 4.45*m_vout;
    Serial.print("Raw Sensor Value = " );
    Serial.print(m_inputvalue);
    Serial.print(" Voltage (V)= " );
    Serial.print(m_vout);
    Serial.print("\n"); 
    return vin;



*/