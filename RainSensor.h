#ifndef RAINSENSOR_H
#define RAINSENSOR_H

#include <arduino.h>

class RainSensor{
    public:
    RainSensor(int digital_pin, int analog_pin);
    int readRain();
    int readMappedRain();

    private:
    int m_digital_pin;
    int m_analog_pin;
    int m_rain_level = 0; // 0,1,2 / No rain/Medium Rain / High rain
    // sensor range
    int m_min = 0;
    int m_max = 1023;
    //helper function
    int readAnalogSignal();
};

#endif //RAINSENSOR_H