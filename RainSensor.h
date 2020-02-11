#ifndef RAINSENSOR_H
#define RAINSENSOR_H

#include <arduino.h>

class RainSensor{
    public:
    RainSensor(int digital_pin, int analog_pin);
    int readRain();

    private:
    int m_digital_pin;
    int m_analog_pin;
    // sensor range
    int m_min = 0;
    int m_max = 1023;
    //helper function
    int readAnalogSignal();
};

#endif //RAINSENSOR_H