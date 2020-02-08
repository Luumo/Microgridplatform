#include "Windsensor.h"


WindSensor::WindSensor(int pin){
    m_pin = pin;
}

void WindSensor::getRawData(){
    m_sensorRawData = analogRead(m_pin);
}


float WindSensor::readWindSpeed(){
    getRawData();
    sensorVoltage = m_sensorRawData * (5.0/1023.0);


    if(sensorVoltage <= voltageMin){
        windSpeed = 0;
    }
    else{
        windSpeed = ((sensorVoltage - voltageMin) / (voltageMax - voltageMin))* 32.0; // meter per second

    }
    return windSpeed;
}