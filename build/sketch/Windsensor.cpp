#include "Windsensor.h"
#include "Sampling.h"

WindSensor::WindSensor(int pin){
    m_pin = pin;
}

void WindSensor::getRawData(){
    m_sensorRawData = data_sampling(analogRead(m_pin), 20);
}


float WindSensor::readWindSpeed(){
    getRawData();
    sensorVoltage = m_sensorRawData * (5.0/1023.0);

    if(sensorVoltage <= voltageMin){
        windSpeed = 0.0;
    }
    else{
        windSpeed = ((sensorVoltage - voltageMin) / (voltageMax - voltageMin))* 32.0; // meter per second

    }
    return windSpeed;
}