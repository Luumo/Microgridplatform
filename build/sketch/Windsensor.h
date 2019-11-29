#ifndef WINDSENSOR_H
#define WINDSENSOR_H
#include <arduino.h>

class WindSensor{
    public:
    WindSensor(int pin);
    void getRawData(int _pin);


    private:
    int _pin;
    int _sensorrawdata = 0; // 0-1023 from analog input

    float voltageMin = 0.4; //
    float windSpeedMin = 0;
    float voltageMax = 2.0;
    float windSpeedMax = 32;

    float windSpeed = 0;

};
#endif //WINDSENSOR_H
