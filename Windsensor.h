#ifndef WINDSENSOR_H
#define WINDSENSOR_H
#include <arduino.h>

class WindSensor{
    public:
    WindSensor(int pin);
    void getRawData();
    float calcWindSpeed();


    private:
    int m_pin;
    float m_sensorRawData = 0.0; // 0-1023 from analog input
    float sensorVoltage = 0.0;

    float voltageMin = 0.4; //
    float windSpeedMin = 0;

    float voltageMax = 2.0;
    float windSpeedMax = 32.0;

    float windSpeed = 0;

};
#endif //WINDSENSOR_H
