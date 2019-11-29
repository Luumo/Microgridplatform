#include "Windsensor.h"


WindSensor::WindSensor(int pin){
    _pin = pin;
}

void WindSensor::getRawData(int _pin){
    _sensorrawdata = analogRead(_pin);
}
