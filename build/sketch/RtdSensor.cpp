#include "RtdSensor.h"

RtdSensor::RtdSensor(int pin){
    m_pin = pin;
}

float RtdSensor::readInputSignal(int m_pin){
    m_ADC = analogRead(m_pin);
}


float RtdSensor::getResistance(){
    readInputSignal(m_pin);
    float resistance = 5000.0 / ( (1023 / m_ADC) -1);
    resistance += 28.0; //thermistor offset
    
    m_resistance = resistance;
}

float RtdSensor::readTemperature(){
    getResistance();
    float temp =((-(R0 * A) +  (sqrt( (R0*A)*(R0*A) + (-4*R0*B*(R0-m_resistance) ) ) ) ) / (2*R0*B)); // formula from PT-1000 doc.
    float offset = (0.30 + 0.005*temp);  
    if (SERIALPRINT){
        Serial.print("Analog reading "); 
        Serial.println(m_ADC);

         Serial.print("Thermistor resistance "); 
        Serial.println(m_resistance);
        
        Serial.print("Temp: "); 
        Serial.println(temp);
        Serial.print("Temp offset: +/- "); 
        Serial.println(offset);
        Serial.print("\n"); 
    }

}


