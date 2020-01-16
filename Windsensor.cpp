#include "Windsensor.h"


WindSensor::WindSensor(int pin){
    m_pin = pin;
}

void WindSensor::getRawData(){
    m_sensorRawData = analogRead(m_pin);
}


float WindSensor::calcWindSpeed(){
    getRawData();
    Serial.print("Analog Data: ");
    Serial.println(m_sensorRawData);
    sensorVoltage = m_sensorRawData * (5.0/1023.0);
    Serial.print("SensorVOltage: ");
    Serial.println(sensorVoltage);

    if(sensorVoltage <= voltageMin){
        windSpeed = 0;
    }
    else{
        windSpeed = ((sensorVoltage - voltageMin) / (voltageMax - voltageMin))* 32.0; // meter per second

    }
    return windSpeed;
}